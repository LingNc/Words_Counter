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