# 文本词频统计系统架构(Words_COUNTER)

## 示例图

![流程图](docs/词频统计.svg)

### 1. 数据输入
- **FileHandle类**
  职责：读取输入文件，不参与数据处理
  输出：原始文本流 → `SqList<string>`

### 2. 数据预处理
- SqList<string>存储结构
  ```
  | hello | world | this | is | a | test | hello | world |
  ```
  - 每个元素为单词
  - 全小写标准化
  - 无标点符号

### 3. 核心处理模块
#### 基类
- **BaseWordCounter**
  抽象接口：
  - `virtual FreqTable count() = 0`
  - `virtual double getASL() = 0`

#### 派生实现类（结构示意图）
```plain
+-----------------------+
|    Implementation     |
+-----------+-----------+
|  Method   |   Type    |
+-----------+-----------+
```

##### 具体实现：
1. **ArrayCounter**
   - 方法：顺序查找 / 二分查找
   - 底层：`SqList<string>`

2. **LinkedListCounter**
   - 方法：链表遍历查找
   - 底层：`SqLink<string>`

3. **OAHashCounter**
   - 方法：开放寻址法哈希查找
   - 底层：`OpenAddrHashTable`

4. **ChainHashCounter**
   - 方法：链地址法哈希查找
   - 底层：`ChainedHashTable`

### 4. 输出结构
- **FreqTable**
  类型：`SqList<pair<string, size_t>>`
  示例：
  ```cpp
  [ ("hello",2), ("world",2), ("test",1) ]
  ```
- **性能指标**
  计算各实现的平均查找长度(ASL)

### 5. 数据输出
- **FileHandle**
  接收`FreqTable` → 写入`output.txt`

## 项目开发

### 1. 开发环境
- **`WorkSpace`**
  使用 Visual Studio Code 的`Workspace`功能，工作区在项目目录中 `Words_Counter.code-workspace` 文件中。
  可以打开该文件，然后点击右下角的`打开工作区`按钮来打开工作区。
- **编译配置**
  使用 `Makefile` 文件来编译项目，支持以下命令：
  - `make main`：编译项目
  - `make test`：编译测试代码
  - `make clean`：清理编译生成的文件
  - `make help` ：显示帮助信息
- **调试配置**
  使用 Visual Studio Code 的调试功能，点击左侧的调试图标，然后选择
    - `调试 Main` 或者 `调试 Test` 进行编译和调试。

### 2. 代码测试
- **测试代码**
  - 测试文件位于 `tests\test.cpp` ，使用 `Makefile` 编译。
  - 测试代码已经被排除，但是纳入编译和调试配置，方便开发使用，适合本地进行代码测试使用
- **测试文件**
  - 使用 `python3 download_tests.py` 来下载测试文件
  - 文件分别是 《哈利波特》 和 《追忆似水年华》
  - 文件名 `HarryPotter.txt` 和 `ÀLaRechercheDuTempsPerdu.txt`