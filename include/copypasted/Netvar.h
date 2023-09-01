#pragma once

#include <unordered_map>
#include <cstring>
#include <memory>
#include "core/logging.hpp"

class RecvTable;
class RecvProp;

class equal_char
{
public:
    bool operator()(const char *const &v1, const char *const &v2) const
    {
        return !strcmp(v1, v2);
    }
};

struct hash_char
{
public:
    size_t operator()(const char *obj) const
    {
        size_t res         = 0;
        const size_t prime = 31;
        for (size_t i = 0; i < strlen(obj); ++i)
        {
            res = obj[i] + (res * prime);
        }
        return res;
    }
};

class netvar_tree
{
    struct node;
    using map_type = std::unordered_map<const char *, std::shared_ptr<node>, hash_char, equal_char>;

    struct node
    {
        node(int offset, RecvProp *p) : offset(offset), prop(p)
        {
        }

        map_type nodes;
        int offset;
        RecvProp *prop;
    };

    map_type nodes;

public:
    void init();

private:
    void populate_nodes(class RecvTable *recv_table, map_type *map);

    static int get_offset_recursive(map_type &map, int acc, const char *name)
    {
        if (!map.count(name))
        {
            logging::Info("can't find %s!", name);
            return 0;
        }
        return acc + map[name]->offset;
    }

    template <typename... args_t> int get_offset_recursive(map_type &map, int acc, const char *name, args_t... args)
    {
        if (!map.count(name))
        {
            logging::Info("can't find %s!", name);
            return 0;
        }
        const auto &node = map[name];
        return get_offset_recursive(node->nodes, acc + node->offset, args...);
    }

    static RecvProp *get_prop_recursive(map_type &map, const char *name)
    {
        return map[name]->prop;
    }

    template <typename... args_t> RecvProp *get_prop_recursive(map_type &map, const char *name, args_t... args)
    {
        const auto &node = map[name];
        return get_prop_recursive(node->nodes, args...);
    }

public:
    template <typename... args_t> int get_offset(const char *name, args_t... args)
    {
        const auto &node = nodes[name];
        if (node == nullptr)
        {
            logging::Info("Invalid NetVar node: %s", name);
            return 0;
        }
        int offset = get_offset_recursive(node->nodes, node->offset, args...);
        return offset;
    }

    template <typename... args_t> RecvProp *get_prop(const char *name, args_t... args)
    {
        const auto &node = nodes[name];
        return get_prop_recursive(node->nodes, args...);
    }

    void dump();
};

extern netvar_tree gNetvars;