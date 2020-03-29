/*  -*- C++ -*-

    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _SUPPORT_H
#define _SUPPORT_H

#define MEMF_ANY    1
#define MEMF_CLEAR  2

void * AllocMem(int size, int type);
void FreeMem(void *ptr, int size);
void CopyMem(const void *src, void *dst, int size);
void SetMem(void *dst, int size, char fill);
int _strcmp_(const char *s1, const char *s2);
int _strlen_(const char *s1);

#if 0
struct MinNode
{
    struct MinNode * mln_Succ,
                   * mln_Pred;
};

struct MinList
{
    struct MinNode * mlh_Head,
                   * mlh_Tail,
                   * mlh_TailPred;
};

#define IsMinListEmpty(l) \
    ( (((struct MinList *)l)->mlh_TailPred) == (struct MinNode *)(l) )

#define NEWLIST(_l)                                     \
do                                                      \
{                                                       \
    struct MinList *__tmp = (struct MinList *)(_l), \
                *l = __tmp;                   \
                                                        \
    l->mlh_TailPred = (struct MinNode *)l;                  \
    l->mlh_Tail     = 0;                                 \
    l->mlh_Head     = (struct MinNode *)&l->mlh_Tail;        \
} while (0)

#define ADDHEAD(_l,_n)                                  \
do                                                      \
{                                                       \
    struct MinNode *__aros_node_tmp = (struct MinNode *)(_n), \
                *n = __aros_node_tmp;                   \
    struct MinList *__aros_list_tmp = (struct MinList *)(_l), \
                *l = __aros_list_tmp;                   \
                                                        \
    n->mln_Succ          = l->mlh_Head;                   \
    n->mln_Pred          = (struct MinNode *)&l->mlh_Head;   \
    l->mlh_Head->mln_Pred = n;                            \
    l->mlh_Head          = n;                            \
} while (0)

#define ADDTAIL(_l,_n)                                    \
do                                                        \
{                                                         \
    struct MinNode *__aros_node_tmp = (struct MinNode *)(_n),   \
                *n = __aros_node_tmp;                     \
    struct MinList *__aros_list_tmp = (struct MinList *)(_l),   \
                *l = __aros_list_tmp;                     \
                                                          \
    n->mln_Succ              = (struct MinNode *)&l->mlh_Tail; \
    n->mln_Pred              = l->mlh_TailPred;             \
    l->mlh_TailPred->mln_Succ = n;                          \
    l->mlh_TailPred          = n;                          \
} while (0)

static inline MinNode * REMOVE(struct MinNode * _n)
{
    struct MinNode *__aros_node_tmp = (struct MinNode *)(_n), *n = __aros_node_tmp;

    n->mln_Pred->mln_Succ = n->mln_Succ;
    n->mln_Succ->mln_Pred = n->mln_Pred;

    return n;
}

static inline MinNode * GetHead(struct MinList * _l)
{
    struct MinList *__aros_list_tmp = (struct MinList *)(_l),
                *l = __aros_list_tmp;

   return l->mlh_Head->mln_Succ ? l->mlh_Head : (struct MinNode *)0;
}

static inline MinNode * GetTail(struct MinList *_l)
{
    struct MinList *__aros_list_tmp = (struct MinList *)(_l),
                *l = __aros_list_tmp;

    return l->mlh_TailPred->mln_Pred ? l->mlh_TailPred : (struct MinNode *)0;
}

static inline MinNode * GetSucc(struct MinNode *_n)
{
    struct MinNode *__aros_node_tmp = (struct MinNode *)(_n),
                *n = __aros_node_tmp;

    return (n && n->mln_Succ && n->mln_Succ->mln_Succ) ? n->mln_Succ : (struct MinNode *)0;
}

static inline MinNode * GetPred(struct MinNode *_n)
{
    struct MinNode *__aros_node_tmp = (struct MinNode *)(_n),
                *n = __aros_node_tmp;

    return (n && n->mln_Pred && n->mln_Pred->mln_Pred) ? n->mln_Pred : (struct MinNode *)0;
}

static inline MinNode * REMHEAD(struct MinList *_l)
{
    struct MinList *__aros_list_tmp = (struct MinList *)(_l),
                *l = __aros_list_tmp;

    return l->mlh_Head->mln_Succ ? REMOVE(l->mlh_Head) : (struct MinNode *)0;
}

static inline MinNode * REMTAIL(struct MinList *_l)
{
    struct MinList *__aros_list_tmp = (struct MinList *)(_l),
                *l = __aros_list_tmp;

    return l->mlh_TailPred->mln_Pred ? REMOVE(l->mlh_TailPred) : (struct MinNode *)0;
}

#define ForeachNode(list, node)                        \
for                                                    \
(                                                      \
    node = (void *)(((struct MinList *)(list))->mlh_Head); \
    ((struct MinNode *)(node))->mln_Succ;                  \
    node = (void *)(((struct MinNode *)(node))->mln_Succ)  \
)

#define ForeachNodeSafe(list, current, next)                  \
    for (                                                     \
        current = (void *)(((struct MinList *)(list))->mlh_Head); \
        (next = (void *)((struct MinNode *)(current))->mln_Succ); \
        current = (void *)next)
#endif

#endif // _SUPPORT_H
