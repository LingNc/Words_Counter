# Makefile for Words_Counter 项目
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

MAIN = WordsCounter
TEST = wordtest

# 目录定义
SRCDIR = src
OBJDIR = build
TESTDIR = tests
INCLUDE = include

# 收集文件
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/src/%.o)

# 创建构建目录
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/src

# 构建主程序
.PHONY : main
main: $(MAIN)
	@echo "构建 $(MAIN) 成功"

# 链接主文件
$(MAIN): $(OBJECTS) $(OBJDIR)/main.o | $(OBJDIR)
	@echo "链接 $(MAIN) ..."
	$(CXX) $(CXXFLAGS) $^ -o $@

# 编译中间产物
$(OBJDIR)/src/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "编译 $< ..."
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

# 编译主程序
$(OBJDIR)/main.o: main.cpp | $(OBJDIR)
	@echo "编译主程序 ..."
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

# 构建测试程序
.PHONY : test
test: $(TEST)
	@echo "构建 $(TEST) 成功"

# 链接测试文件
$(TEST): $(OBJECTS) $(OBJDIR)/test.o | $(OBJDIR)
	@echo "链接 $(TEST) ..."
	$(CXX) $(CXXFLAGS) $^ -o $@

# 编译测试程序
$(OBJDIR)/test.o: $(TESTDIR)/test.cpp | $(OBJDIR)
	@echo "编译测试程序 ..."
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

# 清理中间产物
.PHONY : clean
clean:
	@echo "清理构建文件 ..."
	rm -rf $(OBJDIR)/src/*.o $(MAIN) $(TEST) $(OBJDIR)/main.o $(OBJDIR)/test.o
	rm -rf $(OBJDIR)

# 构建所有目标
.PHONY : all
all: main test
	@echo "构建所有目标成功"

# 运行主程序
.PHONY : run
run: main
	@echo "运行 $(MAIN) ..."
	./$(MAIN)

# 运行测试
.PHONY : run-test
run-test: test
	@echo "运行 $(TEST) ..."
	./$(TEST)

# 调试版本
.PHONY : debug
debug: CXXFLAGS += -DDEBUG -O0
debug: all
	@echo "调试版本构建完成"

# 发布版本
.PHONY : release
release: CXXFLAGS += -DNDEBUG -O3
release: all
	@echo "发布版本构建完成"

# 帮助信息
.PHONY : help
help:
	@echo "可用目标:"
	@echo "  main      - 构建主程序"
	@echo "  test      - 构建测试程序"
	@echo "  all       - 构建所有目标"
	@echo "  run       - 运行主程序"
	@echo "  run-test  - 运行测试程序"
	@echo "  debug     - 构建调试版本"
	@echo "  release   - 构建发布版本"
	@echo "  clean     - 清理构建文件"
	@echo "  help      - 显示帮助信息"
