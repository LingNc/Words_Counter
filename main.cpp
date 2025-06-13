#include "binarysqlist.h"
#include "sequensqlist.h"
#include "chainhash.h"
#include "openhash.h"
#include "linkedlistwordcounter.h"
#include "sqlist.hpp"
#include "filehandle.h"
#include <iostream>
#include <filesystem>
#include <iomanip>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;
namespace fs=std::filesystem;

// 清空屏幕函数
void clear_screen(){
    system("clear");
}

// 初始化所有统计器
SqList<BaseWordCounter *> init_counters(){
    // 初始化所有统计器
    SqList<BaseWordCounter *> counters;
    counters.push_back(new LinkedListWordCounter());
    counters.push_back(new SequenSqList());
    counters.push_back(new BinarySqlist());
    counters.push_back(new ChainHash());
    counters.push_back(new OpenHash());
    return counters;
}

// 查找指定目录中的所有.txt文件
SqList<string> find_txt_files_in(const string &dir){
    SqList<string> txt_files;
    try{
        for(const auto &entry:fs::directory_iterator(dir)){
            if(entry.is_regular_file()&&entry.path().extension()==".txt"){
                txt_files.push_back(entry.path().filename().string());
            }
        }
    }
    catch(const std::exception &e){
        cerr<<"无法读取 "<<dir<<" - "<<e.what()<<endl;
    }
    return txt_files;
}

// 显示文件列表
void show_files(const SqList<string> &files){
    if(files.empty()){
        cout<<"未找到任何.txt文件。\n";
        return;
    }

    cout<<"\n可用的测试文件：\n";
    for(size_t i=0; i<files.size(); ++i){
        cout<<i+1<<". "<<files[i]<<"\n";
    }
}

// 选择文件
string select_file(const SqList<string> &files){
    if(files.empty()) return "";

    show_files(files);

    int choice;
    cout<<"\n请选择文件编号 (1-"<<files.size()<<"): ";
    cin>>choice;

    if(choice < 1||choice > static_cast<int>(files.size())){
        cout<<"无效选择。\n";
        return "";
    }

    return files[choice-1];
}

// 对比多种方法的性能
void compare_methods(const string &filename){
    string filepath="./tests/"+filename;
    FileHandle file_handler(filepath);

    cout<<"\n正在读取文件: "<<filename<<"...\n";
    if(!file_handler.read_from_file()){
        cout<<"文件读取失败！\n";
        return;
    }

    const auto &words=file_handler.get_word_set();
    cout<<"文件读取成功，共找到 "<<words.size()<<" 个单词。\n";

    SqList<BaseWordCounter *> counters=init_counters();

    cout<<"\n开始性能对比...\n";
    cout<<std::setw(20)<<"方法"
        <<std::setw(15)<<"处理时间(ms)"<<"\n";
    cout<<string(50,'-')<<"\n";

    for(auto counter:counters){
        auto start=std::chrono::high_resolution_clock::now();
        counter->load(words);
        auto end=std::chrono::high_resolution_clock::now();

        auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        // const auto &freq_table=counter->get_frequency_table();

        cout<<std::setw(20)<<counter->name()
            <<std::setw(15)<<duration.count()
            <<std::setw(15)<<"\n";
        delete counter;
    }
}

// 单词查找功能
void word_search_menu(const string &selected_file,const SqList<BaseWordCounter *> &counters){
    // 单词查找循环
    while(true){
        clear_screen();
        cout<<"\n=== 单词查找 ===\n";
        cout<<"当前文件: "<<selected_file<<"\n";
        cout<<"请输入要查找的单词（输入 'quit' 退出）: ";

        string search_word;
        cin>>search_word;

        if(search_word=="quit"||search_word=="exit"){
            break;
        }

        cout<<"\n查找单词: '"<<search_word<<"'\n";
        cout<<std::setw(20)<<"方法"
            <<std::setw(10)<<"找到"
            <<std::setw(10)<<"词频"
            <<std::setw(15)<<"比较次数"
            <<std::setw(15)<<"查找时间(μs)"
            <<std::setw(15)<<"ASL"
            <<"\n";
        cout<<string(70,'-')<<"\n";

        for(auto counter:counters){
            auto result=counter->search_word(search_word);

            cout<<std::setw(20)<<counter->name()
                <<std::setw(10)<<(result.isFound?"是":"否")
                <<std::setw(10)<<result.wordFreq
                <<std::setw(15)<<result.comparisons
                <<std::setw(15)<<result.time_us
                <<std::setw(15)<<counter->get_ASL()
                <<"\n";
        }

        if(counters.size()>0&&counters[0]->search_word(search_word).isFound){
            cout<<"\n单词 '"<<search_word<<"' 在文件中出现了 "
                <<counters[0]->search_word(search_word).wordFreq<<" 次。\n";
        }
        else{
            cout<<"\n单词 '"<<search_word<<"' 在文件中未找到。\n";
        }

        cout<<"\n按回车键继续...";
        cin.ignore();
        cin.get();
    }
}

// 输出到文件
void output_into_file(const string &selected_file,FileHandle &file_handler){
    SqList<BaseWordCounter *> counters=init_counters();
    // 创建统计器
    for(int i=1; i<=5; ++i){
        auto &counter=counters[i-1];
        // 载入数据
        const auto &words=file_handler.get_word_set();
        cout<<"统计器: "<<counter->name()<<"\n";
        cout<<"载入文件表生成词频表..."<<endl;
        counter->load(words);

        // 输出结果到文件
        string output_file="./output/"+counter->name()+"_freq.txt";
        if(file_handler.write_into_file(counter->get_frequency_table(),output_file)){
            cout<<"词频统计结果已输出到 "<<output_file<<"\n";
        }
        else{
            cout<<"词频统计结果输出失败。\n";
        }

        delete counter;
    }

    cout<<"\n按回车键继续...";
    cin.ignore();
    cin.get();
}

// 显示前n个词频
void show_top_n(const FreqTable table){
    cout<<"请输入要显示的前n个词频: ";
    size_t n;
    cin>>n;
    if(n==0){
        cout<<"无效的n值，必须大于0。\n";
    }
    // 冒泡排序
    SqList<WordItem> sorted_table=table;
    for(size_t i=0; i<sorted_table.size(); ++i){
        for(size_t j=i+1; j<sorted_table.size(); ++j){
            if(sorted_table[j].second>sorted_table[i].second){
                // 交换
                utils::swap(sorted_table[i],sorted_table[j]);
            }
        }
    }
    cout<<"\n=== 前 "<<n<<" 个词频 ===\n";
    cout<<std::setw(20)<<"单词"
        <<std::setw(10)<<"词频"<<"\n";
    cout<<string(30,'-')<<"\n";

    size_t count=0;
    for(const auto &item:sorted_table){
        if(count>=n) break;
        cout<<std::setw(20)<<item.first
            <<std::setw(10)<<item.second<<"\n";
        ++count;
    }
    // 按任意键继续
    cout<<"\n按回车键继续...";
    cin.ignore();
    cin.get();
}

// 文件处理菜单 - 先选择文件再处理
void file_processing_menu(const SqList<string> &files){
    clear_screen();

    // 选择文件
    string selected_file=select_file(files);
    if(selected_file.empty()){
        cout<<"文件选择失败。\n";
        cout<<"按回车键返回主菜单...";
        cin.ignore();
        cin.get();
        return;
    }

    // 读取文件
    string filepath="./tests/"+selected_file;
    FileHandle file_handler(filepath);

    cout<<"\n正在读取文件: "<<selected_file<<"...\n";
    if(!file_handler.read_from_file()){
        cout<<"文件读取失败！\n";
        cout<<"按回车键返回主菜单...";
        cin.ignore();
        cin.get();
        return;
    }

    const auto &words=file_handler.get_word_set();
    cout<<"文件读取成功，共找到 "<<words.size()<<" 个单词。\n";

    // 创建所有统计器并载入数据
    SqList<BaseWordCounter *> counters=init_counters();
    cout<<"正在载入数据到各个统计器...\n";
    int i=0;
    for(auto counter:counters){
        ++i;
        // 显示导入的第几个
        cout<<i<<" :"
            <<"当前正在载入到统计器: "<<counter->name()<<"\n";
        counter->load(words);
    }
    cout<<"数据载入完成！\n";

    // 从获取单词表
    FreqTable freq_table = counters[1]->get_frequency_table();

    // 文件处理菜单循环
    while(true){
        clear_screen();
        cout<<"\n=== 文件处理菜单 ===\n";
        cout<<"当前文件: "<<selected_file<<"\n";
        cout<<"单词数量: "<<words.size()<<"\n";
        cout<<"1. 单词查找统计\n";
        cout<<"2. 对比所有方法的性能\n";
        cout<<"3. 输出词频表到文件\n";
        cout<<"4. 显示最多的前n词频\n";
        cout<<"5. 返回主菜单\n";
        cout<<"请选择: ";

        int choice;
        cin>>choice;

        if(choice==5) break;

        switch(choice){
        case 1:
            word_search_menu(selected_file,counters);
            break;
        case 2:
            clear_screen();
            compare_methods(selected_file);
            cout<<"\n按回车键继续...";
            cin.ignore();
            cin.get();
            break;
        case 3:
            clear_screen();
            output_into_file(selected_file,file_handler);
            break;
        case 4:
            clear_screen();
            show_top_n(freq_table);
            break;
        default:
            cout<<"无效选择，请重新输入。\n";
            cout<<"按回车键继续...";
            cin.ignore();
            cin.get();
            break;
        }
    }

    // 清理内存
    for(auto counter:counters){
        delete counter;
    }
}

int main(){
    while(true){
        clear_screen();
        cout<<"=== 词频统计系统 ===\n";

        // 检索tests文件夹中的所有.txt文件
        SqList<string> txt_files=find_txt_files_in("./tests");

        if(txt_files.empty()){
            cout<<"在tests文件夹中未找到任何.txt文件。\n";
            cout<<"请确保tests文件夹中有测试文件。\n";
            return 1;
        }

        cout<<"在tests文件夹中找到 "<<txt_files.size()<<" 个.txt文件。\n";

        cout<<"\n=== 主菜单 ===\n";
        cout<<"1. 开始文件检索和词频统计\n";
        cout<<"2. 查看可用文件列表\n";
        cout<<"3. 退出\n";
        cout<<"请选择: ";

        int choice;
        cin>>choice;

        switch(choice){
        case 1:
            file_processing_menu(txt_files);
            break;
        case 2:
            clear_screen();
            show_files(txt_files);
            cout<<"\n按回车键继续...";
            cin.ignore();
            cin.get();
            break;
        case 3:
            clear_screen();
            cout<<"感谢使用词频统计系统，再见！\n";
            return 0;
        default:
            cout<<"无效选择，请重新输入。\n";
            cout<<"按回车键继续...";
            cin.ignore();
            cin.get();
            break;
        }
    }

    return 0;
}