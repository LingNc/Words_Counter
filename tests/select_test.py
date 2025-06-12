#!/usr/bin/env python3
import os
import shutil

def scan_test_files():
    """扫描tests目录中的所有test_*.cpp文件"""
    test_files = []
    tests_dir = "tests"

    for filename in os.listdir(tests_dir):
        if filename.startswith("test_") and filename.endswith(".cpp") and filename != "test.cpp":
            test_files.append(filename)

    return sorted(test_files)

def display_menu(test_files):
    """显示测试文件菜单"""
    print("\n=== 测试文件选择器 ===")
    print("发现以下测试文件:")

    for i, filename in enumerate(test_files, 1):
        # 根据文件名显示描述
        if "sqlist" in filename:
            desc = "顺序表测试"
        elif "hash" in filename:
            desc = "哈希表测试"
        elif "find" in filename:
            desc = "查找功能测试"
        elif "forward" in filename:
            desc = "完美转发测试"
        else:
            desc = "通用测试"

        print(f"[{i}] {filename} - {desc}")

    print("[q] 退出")
    return input("\n请选择要运行的测试 (输入数字或q): ").strip()

def copy_test_file(source_file):
    """将选中的测试文件复制到test.cpp"""
    source_path = os.path.join("tests", source_file)
    target_path = os.path.join("tests", "test.cpp")

    try:
        shutil.copy2(source_path, target_path)
        print(f"✅ 已将 {source_file} 复制到 test.cpp")
        print(f"现在可以运行: make test && ./wordtest")
        return True
    except Exception as e:
        print(f"❌ 复制文件时出错: {e}")
        return False

def main():
    # 切换到项目根目录
    if os.path.basename(os.getcwd()) == "tests":
        os.chdir("..")

    test_files = scan_test_files()

    if not test_files:
        print("❌ 没有找到测试文件")
        return

    while True:
        choice = display_menu(test_files)

        if choice.lower() == 'q':
            print("退出程序")
            break

        try:
            index = int(choice) - 1
            if 0 <= index < len(test_files):
                selected_file = test_files[index]
                print(f"\n选择了: {selected_file}")

                if copy_test_file(selected_file):
                    break
            else:
                print("❌ 无效的选择，请重新输入")
        except ValueError:
            print("❌ 请输入有效的数字")

if __name__ == "__main__":
    main()
