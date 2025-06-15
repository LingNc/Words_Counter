#include "binarysqlist.h"
#include "sequensqlist.h"
#include "chainhash.h"
#include "openhash.h"
#include "linkedlistwordcounter.h"
#include "sqlist.hpp"
#include "filehandle.h"
#include "tools.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>

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
SqList<utils::shared_ptr<BaseWordCounter>> init_counters(){
    // 初始化所有统计器
    SqList<utils::shared_ptr<BaseWordCounter>> counters;
    counters.push_back(utils::shared_ptr<BaseWordCounter>(new LinkedListWordCounter()));
    counters.push_back(utils::shared_ptr<BaseWordCounter>(new SequenSqList()));
    counters.push_back(utils::shared_ptr<BaseWordCounter>(new BinarySqlist()));
    counters.push_back(utils::shared_ptr<BaseWordCounter>(new ChainHash()));
    counters.push_back(utils::shared_ptr<BaseWordCounter>(new OpenHash()));
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
void compare_methods(const string &filename,const SqList<utils::shared_ptr<BaseWordCounter>> counters){
    string filepath="./tests/"+filename;
    FileHandle file_handler(filepath);

    cout<<"\n正在读取文件: "<<filename<<"...\n";
    if(!file_handler.read_from_file()){
        cout<<"文件读取失败！\n";
        return;
    }

    const auto &words=file_handler.get_word_set();
    cout<<"文件读取成功，共找到 "<<words.size()<<" 个单词。\n";

    // 获取词频表用于测试查找性能
    const auto &freq_table = counters[0]->get_frequency_table();
    cout<<"获取到词频表，共 "<<freq_table.size()<<" 个不同单词。\n";

    cout<<"\n开始性能对比...\n";
    cout<<utils::format_aligned("方法",20)
        <<utils::format_aligned("预加载处理时间(ms)",20)
        <<utils::format_aligned("正确平均查找长度(ASL)",25)
        <<utils::format_aligned("平均查找长度",18)
        <<utils::format_aligned("平均查找时间(μs)",18)
        <<"\n";
    cout<<string(101,'-')<<"\n";
    for(auto counter:counters){
        // 测量加载时间
        auto start_load=std::chrono::high_resolution_clock::now();
        // 重新创建并加载数据来测量加载时间，使用智能指针管理
        utils::shared_ptr<BaseWordCounter> temp_counter;
        if(counter->name() == "链表直接查找") {
            temp_counter = utils::shared_ptr<BaseWordCounter>(new LinkedListWordCounter());
        }
        else if(counter->name() == "顺序查找顺序表") {
            temp_counter = utils::shared_ptr<BaseWordCounter>(new SequenSqList());
        }
        else if(counter->name() == "二分查找顺序表") {
            temp_counter = utils::shared_ptr<BaseWordCounter>(new BinarySqlist());
        }
        else if(counter->name() == "链式哈希表") {
            temp_counter = utils::shared_ptr<BaseWordCounter>(new ChainHash());
        }
        else if(counter->name() == "开放地址哈希表") {
            temp_counter = utils::shared_ptr<BaseWordCounter>(new OpenHash());
        }

        if(temp_counter){
            temp_counter->load(words);
        }
        auto end_load=std::chrono::high_resolution_clock::now();
        auto load_duration=std::chrono::duration_cast<std::chrono::milliseconds>(end_load-start_load);

        // 测量查找性能：遍历整个词频表进行查找
        long long total_comparisons=0;
        long long total_search_time_us=0;

        for(const auto &word_item : freq_table){
            auto result = counter->search_word(word_item.first);
            total_search_time_us += result.time_us;
            total_comparisons += result.comparisons;
        }

        // 计算平均值
        double asl=counter->get_ASL();
        double avg_search_time=static_cast<double>(total_search_time_us)/freq_table.size();
        double avg_comparisons=static_cast<double>(total_comparisons)/freq_table.size();

        cout<<utils::format_aligned(counter->name(),20)
            <<utils::format_aligned(std::to_string(load_duration.count()), 20)
            <<utils::format_aligned(std::to_string(asl),25)
            <<utils::format_aligned(std::to_string(avg_comparisons),18)
            <<utils::format_aligned(std::to_string(avg_search_time),18)
            <<"\n";
    }
}

// 单词查找功能
void word_search_menu(const string &selected_file,const SqList<utils::shared_ptr<BaseWordCounter>> &counters){
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
        cout<<utils::format_aligned("方法", 20)
            <<utils::format_aligned("找到", 10)
            <<utils::format_aligned("词频", 10)
            <<utils::format_aligned("比较次数", 15)
            <<utils::format_aligned("查找时间(μs)", 16)
            <<utils::format_aligned("ASL", 15)
            <<"\n";
        cout<<string(86,'-')<<"\n";

        for(auto counter:counters){
            auto result=counter->search_word(search_word);

            cout<<utils::format_aligned(counter->name(), 20)
                <<utils::format_aligned((result.isFound?"是":"否"), 10)
                <<utils::format_aligned(std::to_string(result.wordFreq), 10)
                <<utils::format_aligned(std::to_string(result.comparisons), 15)
                <<utils::format_aligned(std::to_string(result.time_us), 16)
                <<utils::format_aligned(std::to_string(counter->get_ASL()), 15)
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
void output_into_file(const SqList<utils::shared_ptr<BaseWordCounter>>& counters,FileHandle &file_handler){
    // 创建统计器
    for(int i=1; i<=5; ++i){
        auto &counter=counters[i-1];

        // 输出结果到文件
        string output_file="./output/"+counter->name()+"_freq.txt";
        if(file_handler.write_into_file(counter->get_frequency_table(),output_file)){
            cout<<"词频统计结果已输出到 "<<output_file<<"\n";
        }
        else{
            cout<<"词频统计结果输出失败。\n";
        }
    }

    cout<<"\n按回车键继续...";
    cin.ignore();
    cin.get();
}

// 自定义排序函数
// 用于显示前n个词频
bool compare_word_items(const WordItem &a, const WordItem &b) {
    return a.second > b.second; // 按词频降序排序
}

// 显示前n个词频
void show_top_n(const SqList<utils::shared_ptr<BaseWordCounter>> &counters){
    // 获取单词表
    auto table = counters[3]->get_frequency_table();

    cout<<"请输入要显示的前n个词频: ";
    size_t n;
    cin>>n;
    if(n==0){
        cout<<"无效的n值，必须大于0。\n";
    }
    // 排序
    static SqList<WordItem> sorted_table=table;
    utils::sort(sorted_table.begin(),sorted_table.end(),compare_word_items);

    cout<<"\n=== 前 "<<n<<" 个词频 ===\n";
    cout<<utils::format_aligned("单词", 20)
        <<utils::format_aligned("词频", 10)<<"\n";
    cout<<string(30,'-')<<"\n";

    size_t count=0;
    for(const auto &item:sorted_table){
        if(count>=n) break;
        cout<<utils::format_aligned(item.first, 20)
            <<utils::format_aligned(std::to_string(item.second), 10)<<"\n";
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
    SqList<utils::shared_ptr<BaseWordCounter>> counters=init_counters();
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
            compare_methods(selected_file,counters);
            cout<<"\n按回车键继续...";
            cin.ignore();
            cin.get();
            break;
        case 3:
            clear_screen();
            output_into_file(counters,file_handler);
            break;
        case 4:
            clear_screen();
            show_top_n(counters);
            break;
        default:
            cout<<"无效选择，请重新输入。\n";
            cout<<"按回车键继续...";
            cin.ignore();
            cin.get();
            break;
        }
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