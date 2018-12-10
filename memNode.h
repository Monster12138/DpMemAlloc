#ifndef _MEMNODE_H_
#define _MEMNODE_H_
#include <iostream>

class memNode
{
public:
    memNode() {}

    int get_begin()const { return _begin; }

    int get_end()const { return _end; }

    int get_size()const { return _size; }

    void set_node(int begin, int end){
        _begin = begin;
        _end = end;
        _size = _end - _begin;
    };

    void cut_node(int n){
        _begin += n;
        _size = _end- _begin;
    }

    ~memNode() {}
private:
    int _begin;
    int _end;
    int _size;
};


#endif
