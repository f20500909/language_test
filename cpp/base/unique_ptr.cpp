#include <iostream>
#include <memory>

struct Task
{
    int mId;
    // Consutuctor
    Task(int id) : mId(id)
    {
        std::cout << "Task::Constructor" << std::endl;
    }
    ~Task()
    {
        std::cout << "Task::Destructor" << std::endl;
    }
};

int main()
{
    // 通过原始指针创建 unique_ptr 实例
    // std::unique_ptr<Task> taskPtr(new Task(23));
    std::unique_ptr<Task> taskPtr(new Task(1));


       std::unique_ptr<int> pInt2(taskPtr);    // 报错
    std::unique_ptr<int> pInt3 = taskPtr;   // 报错

    //通过 unique_ptr 访问其成员
    int id = taskPtr->mId;
    std::cout << id << std::endl;

    return 0;
}
