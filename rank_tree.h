#ifndef RANK_TREE_H
#define RANK_TREE_H

#include <iostream>
#include <memory>
#include <assert.h>

class AvlRankTree
{         
    private:
    struct Node{
        friend class AvlRankTree;
        int salary;
        int employeeId;
        int employeeGrade;
        int employeeCompanyId;

        struct Node*  father;
        struct Node*  left;
        struct Node*  right;
        int height;
        int numPeopleAtSubTree;
        int sumOfGradesInSubTree;
        int size; //amount of Nodes in sub tree- equal to n(v)
    };
    int size_of_tree; //amount of nodes in all tree
    Node* root;
    Node* max;
    Node* min; 

    /*operation for Node*/
    int calculateBalanceFactor(Node* node) const;
    Node* getMinNode(Node* node) const;
    Node* getMaxNode(Node* node) const;
    int getNodeSalary(Node* node) const;
    int getNodeCompanyId(Node* node) const;
    int getNodeEmployeeId(Node* node) const;
    int getNodeEmployeeGrade(Node* node) const;
    int getNodeSize(const Node* node) const;
    int getNodeHeight(const Node* node) const;
    int getNodeNumPeopleAtAtSubTree(const Node* node) const;
    int getNodeSumOfGradesInSubTree(const Node* node) const;
    
    Node* createNode(int salary, int employee_id, int company_id, int grade);//was (int level)
    Node* duplicateNode(Node* node);
    Node* insertNode(Node* new_node,Node* target,Node* parent);
    Node* findNode(Node* node, int salary, int employeeId) const;
    Node* deleteNode(Node* target, Node* to_remove);
    Node* selectNodeOperation(Node* node, int index) const;
    void printTreeAux(Node* root) const;
    void sumUntilReachHighLevel(Node* start,int highLevel, long long* with_score,int* in_range);
    void sumUntilReachLowLevel(Node* start,int lowLevel, long long* with_score,int* in_range);

    /*roll function*/
    Node* rollLeftLeft(Node* node);
    Node* rollRightRight(Node* node);
    Node* rollRightLeft(Node* node);
    Node* rollLeftRight(Node* node);

    // helper func for Constructor && Destructor of AVL
    void emptyTreeAux(Node* node);
    Node* convertSortedArrayToTreeAux(Node* sorted_array[],int start,int end,Node* father);

    //avl rank tree's constructor, destructor, interface functions
    public:
    bool isTreeEmpty() const;
    void empty();
    AvlRankTree();
    AvlRankTree(const AvlRankTree& other);
    ~AvlRankTree();
    AvlRankTree(const AvlRankTree& firstTree, const AvlRankTree& secondTree); //merge
    AvlRankTree& operator=(const AvlRankTree &other);
    void convertSortedArrayToTree(Node* dataArr[],int arrSize);
    void insert(int salary, int employeeId, int companyId, int grade);
    Node* find(int salary, int employeId);
    void remove(int salary, int employeeId);
    Node* select(int index) const;
    int getHeight() const;
    void printTree() const; //for debug
    void fillFromTree(const AvlRankTree& src); //O(n) //fill a tree from data of other tree
    Node* getCommonFather(Node* first,Node* second,Node* origin);
    double getAverageBumpGradeBetweenSalaryByGroup( int lowLevel, int highLevel, int* error,
        int sumGradeZero, long long numPeopleAtZero);
    int getTotalNumPeopleAtTree() const;
    int getSumOfHighestMPlayerLevel(int sum,int m,Node* node);
    Node* getRoot() const;

    //iterator-in order to sort the elements into an array
    class Iterator
    {
        private:
        friend class AvlRankTree;
        Node* node;
        Node* last;
        public:
        Iterator(Node* node, Node* last);
        ~Iterator()=default;
        Iterator& operator=(const Iterator& other_iter);
        Iterator(const Iterator& other_iter);
        Node* operator*();
        bool operator==(const Iterator& iter) const;
        bool operator!=(const Iterator& iter) const;
        Iterator& operator++();
    };
    Iterator begin() const;
    Iterator end() const;

};


#endif