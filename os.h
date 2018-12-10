#ifndef _OS_H_
#define _OS_H_

#include <list>
#include <algorithm>
#include <iostream>
#include "memNode.h"
using namespace std;

enum flags{
    SIZE_ASC,
    SIZE_DESC,
    ADDR_ASC,
    NO_LOOP,
    LOOP
};
/*
const int SIZE_DESC = 0;
const int SIZE_ASC = 1;
const int ADDR_ASC = 2;
const int NO_LOOP = 3;
const int LOOP = 4;
*/
class os
{
public:
    os():_initNum(0),_minSize(0), _empty(), _use() {}

    void init()
    {    
        cout << "请输入最小不可分割大小:>";
        cin >> _minSize;

        cout << "请输入初始内存块数:>";
        cin >> _initNum;
    }

    void create_empty(){
        for(int i = 0; i < _initNum; ++i){
            memNode *newNode = new memNode;
            int begin, end;
            cout << "请输入第[" << i+1 << "]个空白内存块的首尾地址:>";
            cin >> begin >> end;
            newNode->set_node(begin, end);
            _empty.push_back(newNode);
        }
    }

    //flag 1 降序　0 升序  2 地址升序
    void list_sort(int flag){
        switch(flag){
        case SIZE_DESC:_empty.sort(less_than);break;
        case SIZE_ASC:_empty.sort(greater);break;
        case ADDR_ASC:_empty.sort(addr);break;
        default:return;
        }
    }

    bool alloc(int size, list<memNode*>::iterator& start, int flag){
        list<memNode*>::iterator it = start;

        for(; it != _empty.end(); ++it){
            if((*it)->get_size() - size > _minSize){
                memNode *newNode = new memNode;
                newNode->set_node((*it)->get_begin(), (*it)->get_begin() + size);
                (*it)->cut_node(size);
                _use.push_back(newNode);
                start = ++it;
                return true;
            }
            else if((*it)->get_size() - size >= 0){
                _use.push_back(*it);
                _empty.erase(it);
                start = ++it;
                return true;
            }
        }
        if(it == _empty.end()){
            if(flag == NO_LOOP)
                return false;
            else {
                for(it = _empty.begin(); it != start; ++it){
                    if((*it)->get_size() - size > _minSize){
                        memNode *newNode = new memNode;
                        newNode->set_node((*it)->get_begin(), (*it)->get_begin() + size);
                        (*it)->cut_node(size);
                        _use.push_back(newNode);
                        start = ++it;
                        return true;
                    }
                    else if((*it)->get_size() - size >= 0){
                        _use.push_back(*it);
                        _empty.erase(it);
                        start = ++it;
                        return true;
                    }
                }
                if(it == start)
                    return false;
            }
        }

        return true;
    }

    /*
    bool loop_alloc(int size, list<memNode*>::iterator it){
        auto start = it;
        for(; it != _empty.end(); ++it){
            if((*it)->get_size() - size > _minSize){
                memNode *newNode = new memNode;
                newNode->set_node((*it)->get_begin(), (*it)->get_begin() + size);
                (*it)->cut_node(size);
                _use.push_back(newNode);
                return true;
            }
            else if((*it)->get_size() - size >= 0){
                _use.push_back(*it);
                _empty.erase(it);
                return true;
            }
        }
        if(it == _empty.end())return false;

        return true;
    }    
    */
    void output_list(const list<memNode*>& list){
        for(auto e:list){
            printf("┌──────┬──────┐   ");
        }
        printf("┌──────┐");
        cout << endl;
        for(auto e:list){
            //printf("┌──────┬──────┐\n");
            printf("│%6d│%6d│——>", e->get_begin(), e->get_end());
            //printf("└──────┴──────┘\n");
        }
        printf("│ null │");
        cout << endl;
        for(auto e:list){
            printf("└──────┴──────┘   ");
        }
        printf("└──────┘");
        cout << endl;
    }

    void output(){
        cout << endl;
        cout << "空闲分区链:\n";
        output_list(_empty);

        cout << "已用分区链:\n";
        output_list(_use);
        cout << endl;
    }

    void distribution(int flag){
        int size;
        cout << "请输入要到来进程需要的内存大小:>";
        cin >> size;
        static auto sit = _empty.begin();
        auto it = _empty.begin();
        bool ret;
        if(flag == LOOP){
            ret = alloc(size, sit, flag);
        }
        else {
            ret = alloc(size, it, flag);
        }
        if(ret == false){
            cout << "可用内存不足，分配失败!!" << endl;
            return;
        }
    }

    void freed(){
        int addr;
        cout << "请输入要释放内存块的起始地址:";
        cin >> addr;
        auto it = _use.begin();
        for( ;it != _use.end(); ++it){
            if((*it)->get_begin() == addr){
                _empty.push_back(*it);
                _use.erase(it);
                return;
            }
        }
        if(it == _use.end()){
            output();
            cout << "请输入正确的起始地址!" << endl;
        }
    }

    void merge(list<memNode*> &l){
        auto it = l.begin();
        for( ;it != l.end(); ++it){
            auto tmp = it;
            ++it;
            if(it == l.end())break;
            if((*tmp)->get_end() == (*it)->get_begin()){
                (*tmp)->set_node((*tmp)->get_begin(), (*it)->get_end());
                l.erase(it);
            }
            it = tmp;
        }
    }

    void FF(){
        merge(_empty);
        list_sort(ADDR_ASC);
        output();
        int c;
        while(1){
            cout << "1.分配　　2.释放    0.退出\n请选择:>";
            cin >> c;
            switch(c){
            case 1:distribution(NO_LOOP);break;
            case 2:freed();break;
            case 0:return;
            default:continue;
            }
            list_sort(ADDR_ASC);
            merge(_empty);
            output();
        }
    }

    void NF(){
        merge(_empty);
        list_sort(ADDR_ASC);
        output();
        int c;
        while(1){
            cout << "1.分配　　2.释放    0.退出\n请选择:>";
            cin >> c;
            switch(c){
            case 1:distribution(LOOP);break;
            case 2:freed();break;
            case 0:return;
            default:continue;
            }
            list_sort(ADDR_ASC);
            merge(_empty);
            output();
        }
    }

    void BF(){
        list_sort(ADDR_ASC);
        merge(_empty);
        list_sort(SIZE_ASC);
        output();
        int c;
        while(1){
            cout << "1.分配　　2.释放    0.退出\n请选择:>";
            cin >> c;
            switch(c){
            case 1:distribution(NO_LOOP);break;
            case 2:freed();break;
            case 0:return;
            default:continue;
            }
            list_sort(ADDR_ASC);
            merge(_empty);
            list_sort(SIZE_ASC);
            output();
        }
    }

    void WF(){
        list_sort(ADDR_ASC);
        merge(_empty);
        list_sort(SIZE_DESC);
        output();
        int c;
        while(1){
            cout << "1.分配　　2.释放    0.退出\n请选择:>";
            cin >> c;
            switch(c){
            case 1:distribution(NO_LOOP);break;
            case 2:freed();break;
            case 0:return;
            default:continue;
            }
            list_sort(ADDR_ASC);
            merge(_empty);
            list_sort(SIZE_DESC);
            output();
        }
    }

    ~os() {
        for(auto e:_empty){
            delete e;
        }
        for(auto e:_use){
            delete e;
        }
    }

private:
    int _initNum;
    int _minSize;
    list<memNode*> _empty;
    list<memNode*> _use;
    struct less_than{
        bool operator()(const memNode* n1,const memNode* n2){
            if(n1->get_size() > n2->get_size())return true;
            return false;
        }
    }less_than;

    struct greater{
        bool operator()(const memNode* n1,const memNode* n2){
            if(n1->get_size() > n2->get_size())return true;
            return false;
        }
    }greater;

    struct addr{
        bool operator()(const memNode* n1, const memNode* n2){
            if(n1->get_begin() < n2->get_begin())return true;
            return false;
        }
    }addr;
};

#endif
