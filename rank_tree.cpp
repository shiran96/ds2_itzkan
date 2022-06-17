#include "rank_tree.h"

static int getMax(int first, int second)
{
    return (first>second)?first:second;
}

int AvlRankTree::calculateBalanceFactor(Node* node) const
{
    return getNodeHeight(node->left) - getNodeHeight(node->right);
}

AvlRankTree::Node* AvlRankTree::getMinNode(Node* node) const
{   
    if(node==nullptr)
    {
        return nullptr;
    }
    if(node->left==nullptr)
    {
        return node;
    }
    return getMinNode(node->left);
}

AvlRankTree::Node* AvlRankTree::getMaxNode(Node* node) const
{
    if(node==nullptr)
    {
        return nullptr;
    }
    if(node->right==nullptr)
    {
        return node;
    }
    return getMaxNode(node->right);   
}

int AvlRankTree::getNodeSalary(Node* node) const
{
    return (node==nullptr)?-1:node->salary;
}

int AvlRankTree::getNodeSize(const Node* node) const
{   
    return (node==nullptr)?0:node->size;
}

int AvlRankTree::getNodeHeight(const Node* node) const
{
    return (node==nullptr)?0:node->height;
}

int AvlRankTree::getNodeNumPeopleAtAtSubTree(const Node* node) const
{
    return (node==nullptr)?0:node->numPeopleAtSubTree;
}
int AvlRankTree::getNodeSumOfGradesInSubTree(const Node* node) const
{
    return (node==nullptr)?0:node->sumOfGradesInSubTree;
}

int AvlRankTree::getNodeCompanyId(Node* node) const
{
    return (node == nullptr) ? 0 : node->employeeCompanyId;
}

int AvlRankTree::getNodeEmployeeId(Node* node) const
{
    return (node == nullptr) ? 0 : node->employeeId;
}

int AvlRankTree::getNodeEmployeeGrade(Node* node) const
{
    return (node == nullptr) ? 0 : node->employeeGrade;
}

AvlRankTree::Node* AvlRankTree::createNode(int salary, int employee_id, int company_id, int grade)
{
    Node* new_node=new Node;
    new_node->salary=salary;
    new_node->employeeId = employee_id;
    new_node->employeeCompanyId = company_id;
    new_node->employeeGrade = grade;
    new_node->father=nullptr;
    new_node->left=nullptr;
    new_node->right=nullptr;
    new_node->height=0;
    new_node->size=0;
    new_node->numPeopleAtSubTree=0;
    new_node->sumOfGradesInSubTree =0;
    
    return new_node;
}

AvlRankTree::Node* AvlRankTree::duplicateNode(Node* node)
{
    if(node==nullptr)
    {
        return nullptr;
    }
    Node* left_copy=duplicateNode(node->left);
    Node* right_copy=duplicateNode(node->right);
    Node* new_node=createNode(node->salary,node->employeeId,node->employeeCompanyId,node->employeeGrade);
    
    new_node->left=left_copy;
    if(new_node->left!=nullptr)
    {
        new_node->left->father=new_node;
    }
    new_node->right=right_copy;
    if(new_node->right!=nullptr)
    {
        new_node->right->father=new_node;
    }
    new_node->size=getNodeSize(new_node->right)+getNodeSize(new_node->left)+1;
    new_node->height=getNodeHeight(new_node->right)+getNodeHeight(new_node->left)+1;
    new_node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(new_node->right)
        +getNodeNumPeopleAtAtSubTree(new_node->left)+1;
    new_node->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(new_node->right)
        + getNodeSumOfGradesInSubTree(new_node->left)
        +(new_node->sumOfGradesInSubTree)*(new_node->employeeGrade);
    
    return new_node;
}

AvlRankTree::Node* AvlRankTree::insertNode(Node* new_node,Node* target,Node* parent)
{
    /*insert like in a binary tree before fixing by rolling*/ 
    if(target==nullptr)
    {
        new_node->father=parent;
        new_node->size=getNodeSize(new_node->left)+getNodeSize(new_node->right)+1;
        new_node->height=getNodeHeight(new_node->right)+getNodeHeight(new_node->left)+1;
        new_node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(new_node->left)
        +getNodeNumPeopleAtAtSubTree(new_node->right)+1; 
        new_node->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(new_node->left)
        + getNodeSumOfGradesInSubTree(new_node->right)
        +(new_node->employeeGrade);
        
        return new_node;
    }
    if(new_node->salary < target->salary) 
    {
        target->left=insertNode(new_node,target->left,target);
    }
    else if(new_node->salary > target->salary)
    {     
        target->right=insertNode(new_node,target->right,target);
    }
    else //if new_node->salary == target->salary
    {
        if (new_node->employeeId > target->employeeId)
        {
            target->right = insertNode(new_node, target->right, target);
        }
        else //we assume new_node->employeeId != target->employeeId
        {
            target->left = insertNode(new_node, target->left, target);
        }
    }

    /*balance the tree- fix by rolling,
    will only get here and enter the if-condition if a balance was changed 
    after adding a node to the tree, will correct recursively until the root*/ 
    if(calculateBalanceFactor(target)>1)  //changed
    {
        if(calculateBalanceFactor(target->left)>=0)
        {
            return rollLeftLeft(target);
        }
        else //BF(root->left)=-1
        {
            return rollLeftRight(target);
        }
    }
    else if(calculateBalanceFactor(target)<-1) //changed
    {
        if(calculateBalanceFactor(target->right)<=0)
        {
            return rollRightRight(target);
        }
        else //BF(root->right)=1
        {
            return rollRightLeft(target);
        }
    }
    /*correct height and other fields*/
    target->height=getMax(getNodeHeight(target->right),getNodeHeight(target->left))+1;
    target->size=getNodeSize(target->left)+getNodeSize(target->right)+1;
    target->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(target->right)+
        getNodeNumPeopleAtAtSubTree(target->left)+1;
    target->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(target->right)
        + getNodeSumOfGradesInSubTree(target->left)
        +(target->employeeGrade);
    
    return target;

}

AvlRankTree::Node* AvlRankTree::findNode(Node* node,int salary, int employeeId) const
{
    if(node==nullptr)
    {
        return nullptr; //if not in the tree
    }
    if(node->salary==salary && node->employeeId== employeeId)
    {
        return node;
    }
    if(node->salary > salary)
    {
        return findNode(node->left,salary, employeeId);
    }
    if(node->salary < salary)
    {
        return findNode(node->right, salary, employeeId);
    }
    if (node->salary == salary)
    {
        if (node->employeeId < employeeId)
        {
            return findNode(node->right, salary, employeeId);
        }
        else //node->employeeId > employeeId
        {
            return findNode(node->left, salary, employeeId);
        }
    }
    return nullptr;
}

AvlRankTree::Node* AvlRankTree::deleteNode(Node* target, Node* to_remove)
{
    if(target==nullptr || to_remove== nullptr)  //if(to_remove==nullptr)-did segmentation
    {                                          //need to check target not to remove...
        return nullptr;
    }
    //remove the node
    
    if (to_remove->salary == target->salary && to_remove->employeeId == target->employeeId)//if target==to_remove
       
    {
        if(target->left==nullptr && target->right==nullptr) //a leaf
        {
            delete target;
            return nullptr;
        }
        else if(target->left==nullptr)
        {
            Node* tempNode=target->right;   
            target->salary=tempNode->salary;
            target->employeeId = tempNode->employeeId;
            target->employeeCompanyId = tempNode->employeeCompanyId;
            target->employeeGrade = tempNode->employeeGrade;

            target->right=deleteNode(target->right,target);
            //i dont think we need to update more fields 
            //if there is an error- check if need to update
            //num people at level and scoreGroupArr from tempNode
        }
        else if(target->right==nullptr)
        {
            Node* tempNode=target->left;
            target->salary = tempNode->salary;
            target->employeeId = tempNode->employeeId;
            target->employeeCompanyId = tempNode->employeeCompanyId;
            target->employeeGrade = tempNode->employeeGrade;

            target->left = deleteNode(target->left,target);
        }
        else //there is a left son and a right son
        {
            Node* tempNode=getMinNode(target->right);
            target->salary = tempNode->salary;
            target->employeeId = tempNode->employeeId;
            target->employeeCompanyId = tempNode->employeeCompanyId;
            target->employeeGrade = tempNode->employeeGrade;
            
            target->right=deleteNode(target->right,target);
        }
    }
    else if(to_remove->salary > target->salary)
    {
        target->right=deleteNode(target->right,to_remove);
    }
    else if (to_remove->salary < target->salary)
    {
        target->left=deleteNode(target->left,to_remove);
    }
    else //to_remove->salary == target->salary
    {
        if (to_remove->employeeId < target->employeeId)
        {
            target->left = deleteNode(target->left, to_remove);
        }
        else // to_remove->employeeId > target->employeeId 
        // we assume to_remove->employeeId != target->employeeId,
        //because of first check
        {
            target->right = deleteNode(target->right, to_remove);
        }

    }


    //fix
    target->height=getMax(getNodeHeight(target->right),getNodeHeight(target->left))+1;
    target->size=getNodeSize(target->right)+getNodeSize(target->left)+1;
    target->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(target->right)
        +getNodeNumPeopleAtAtSubTree(target->left)+1;
    target->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(target->right)
        + getNodeSumOfGradesInSubTree(target->left)
        +(target->employeeGrade);
    
    //balance the tree
    if(calculateBalanceFactor(target)>1) //changed
    {
        if(calculateBalanceFactor(target->left)>=0)
        {
            return rollLeftLeft(target);
        }
        else
        {
            return rollLeftRight(target);
        }
    }
    else if(calculateBalanceFactor(target)<-1)  //changed
    {
        if(calculateBalanceFactor(target->right)==1)
        {
            return rollRightLeft(target);
        }
        else
        {
            return rollRightRight(target);
        }
    }
    return target;   
    }

AvlRankTree::Node* AvlRankTree::selectNodeOperation(Node* node, int index) const
{
    if(getNodeSize(node->left)==index-1) //the prev to node got the prev index
    {
        return node;
    }
    else if(getNodeSize(node->left) < index-1) // go more left
    {
        return selectNodeOperation(node->right, index- (getNodeSize(node->left)+1));
    }
    else  //go more right to biggers ranks
    {
        return selectNodeOperation(node->left,index);
    }   
}

AvlRankTree::Node* AvlRankTree::rollLeftLeft(Node* node)
{ //rotate
    Node* tempNode = node->left;
    node->left->father=node->father;
    node->father=tempNode;
    if(tempNode->right!=nullptr)
    {
        tempNode->right->father=node;
    }
    node->left =node->father->right;
    node->father->right=node;
    //update height and other fields
    node->height=getMax(getNodeHeight(node->left),getNodeHeight(node->left))+1;
    node->size=getNodeSize(node->right)+getNodeSize(node->left)+1;
    node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->right)
        +getNodeNumPeopleAtAtSubTree(node->left)+1;
    node->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(node->right)
        + getNodeSumOfGradesInSubTree(node->left)
        +(node->employeeGrade);
    
    if(node->father != nullptr)
    {
        node->father->height=getMax(getNodeHeight(node->father->right),getNodeHeight(node->father->left))+1;
        node->father->size=getNodeSize(node->father->right)+getNodeSize(node->father->left)+1;
        node->father->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->father->right)
        +getNodeNumPeopleAtAtSubTree(node->father->left)+1;
        node->father->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(node->father->right)
        + getNodeSumOfGradesInSubTree(node->father->left)
        +(node->father->sumOfGradesInSubTree);
        
    }
    return node->father;
}
AvlRankTree::Node* AvlRankTree::rollRightRight(Node* node)
{
    //rotate
    Node* temp=node->right;
    node->right->father=node->father;
    node->father=temp;
    node->right=node->father->left;
    if(node->right!=nullptr)
    {
        node->right->father = node;
    }
    node->father->left = node;
    //update heights and such
    node->size=getNodeSize(node->right)+getNodeSize(node->left)+1;
    node->height=getNodeHeight(node->right)+getNodeSize(node->left)+1;
    node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->right)
        +getNodeNumPeopleAtAtSubTree(node->left)+1;
    node->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(node->right)
        + getNodeSumOfGradesInSubTree(node->left)
        +(node->employeeGrade);
    
    if(node->father != nullptr)
    {
        node->father->height=getMax(getNodeHeight(node->father->right),getNodeHeight(node->father->left))+1;
        node->father->size=getNodeSize(node->father->right)+getNodeSize(node->father->left)+1;
        node->father->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->father->right)
        +getNodeNumPeopleAtAtSubTree(node->father->left)+1;
        node->father->sumOfGradesInSubTree=getNodeSumOfGradesInSubTree(node->father->right)
        + getNodeSumOfGradesInSubTree(node->father->left)
        +(node->father->employeeGrade);
        
    }
    return node->father;  
}

//combination of a single left rotation followed by a single right rotation
AvlRankTree::Node* AvlRankTree::rollRightLeft(Node* node)
{
    node->right = rollLeftLeft(node->right);
    return rollRightRight(node);
}

//combination of a single right rotation followed by a single left rotation
AvlRankTree::Node* AvlRankTree::rollLeftRight(Node* node)
{
    node->left = rollRightRight(node->left);
    return rollLeftLeft(node);
}

void AvlRankTree::emptyTreeAux(Node* node)
{
    if(node==nullptr)
    {
        return;
    }
    if(node->right!=nullptr)
    {
        emptyTreeAux(node->right);
    }
    if(node->left!=nullptr)
    {
        emptyTreeAux(node->left);
    }
    
    delete node;
    return;
}

AvlRankTree::Node* AvlRankTree::convertSortedArrayToTreeAux(Node* sorted_array[],int start,int end,Node* father)
{
    if(start>end)
    {
        return nullptr;
    }
    int middle=(start+end)/2;
    Node* new_element_in_tree = createNode(getNodeSalary(sorted_array[middle]), getNodeEmployeeId(sorted_array[middle])
        , getNodeCompanyId(sorted_array[middle]), getNodeEmployeeGrade(sorted_array[middle]));
    
    new_element_in_tree->father=father;
    new_element_in_tree->right = convertSortedArrayToTreeAux(sorted_array, middle + 1, end, new_element_in_tree);
    new_element_in_tree->left = convertSortedArrayToTreeAux(sorted_array, start, middle - 1, new_element_in_tree);
    new_element_in_tree->height=getMax(getNodeHeight(new_element_in_tree->right),getNodeHeight(new_element_in_tree->left))+1;
    new_element_in_tree->size =getNodeSize(new_element_in_tree->right)+getNodeHeight(new_element_in_tree->left)+1;
    new_element_in_tree->numPeopleAtSubTree= getNodeNumPeopleAtAtSubTree(new_element_in_tree->right)
        + getNodeNumPeopleAtAtSubTree(new_element_in_tree->left)
        +1;
    new_element_in_tree->sumOfGradesInSubTree= getNodeSumOfGradesInSubTree(new_element_in_tree->right)
        + getNodeSumOfGradesInSubTree(new_element_in_tree->left)
        +(new_element_in_tree->employeeGrade);
    
    return new_element_in_tree;
}

void AvlRankTree::printTreeAux(Node* root) const //print inorder style
{
    if(root==nullptr)
    {
        return;
    }
    printTreeAux(root->left);
    std::cout<< "salary:"<< root->salary <<std::endl;
    std::cout << "Id:" << root->employeeId << std::endl;
    std::cout<<"num of people in sub tree:"<<root->numPeopleAtSubTree<<std::endl;
    std::cout<<"sum of grades in sub tree:"<<root->sumOfGradesInSubTree <<std::endl;
    
    std::cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"<<std::endl;
    printTreeAux(root->right);
}

bool AvlRankTree::isTreeEmpty() const
{
    return (size_of_tree==0);
}
void AvlRankTree::empty()
{
    if(size_of_tree > 0)
    {
    emptyTreeAux(root);
    }
    root = nullptr;
    size_of_tree = 0;
    max = nullptr;
    min=nullptr;
}

AvlRankTree::AvlRankTree():size_of_tree(0),root(nullptr),max(nullptr),min(nullptr){}

AvlRankTree::AvlRankTree(const AvlRankTree& other):size_of_tree(other.size_of_tree),root(nullptr),max(nullptr),min(nullptr)
{
    //size_of_tree=other.size_of_tree;
    root=duplicateNode(other.root);
    max=getMaxNode(root);
    min=getMinNode(root);
}

AvlRankTree::~AvlRankTree()
{
    empty();  
}

AvlRankTree& AvlRankTree::operator=(const AvlRankTree &other)
{
    empty();
    root = duplicateNode(other.root);
    size_of_tree = other.size_of_tree;
    max=getMaxNode(root);
    min=getMinNode(root);
    return *this;
}

//it's on the user that the tree will be empty 
void AvlRankTree::convertSortedArrayToTree(Node* dataArr[],int arrSize)
{
    root=convertSortedArrayToTreeAux(dataArr,0,arrSize-1,nullptr);
    size_of_tree=arrSize;
    max=getMaxNode(root);
    min=getMinNode(root);
}

//we call this function knowing the data of the certain player isnt in the tree
void AvlRankTree::insert(int salary, int employeeId, int companyId, int grade)
{
    Node* level_node=findNode(root,salary, employeeId);
    //if the level doesnt exist- create and insert a new level to the tree
    //with the info of the player
    if(level_node==nullptr)
    {
        Node* new_level=createNode(salary, employeeId, companyId, grade);
        
        root=insertNode(new_level,root,nullptr);
        min = getMinNode(root);
        max = getMaxNode(root);
        size_of_tree++;
    }
    else //the level exist-> make a copy with same GroupScoreArr and numPeopleAtLevel
    {   //delete the level and reInsert after updating and adding the new data
        Node* updated_level_node= createNode(salary, employeeId, companyId, grade);//groupScoreArr empty, numOfPeopleAtlevel=0

        root=deleteNode(root,level_node);
        max= getMaxNode(root);
        min= getMinNode(root);
        size_of_tree--;
        root=insertNode(updated_level_node,root,nullptr);
        min = getMinNode(root);
        max = getMaxNode(root);
        size_of_tree++;
    }
}


AvlRankTree::Node* AvlRankTree::find(int salary,int employeId)
{
    Node* node=findNode(root,salary, employeId);
    return node; //may be null
}

//we call this function knowing the data of the certain player is in the tree
void AvlRankTree::remove(int salary,int employeeId)
{
    Node* to_delete=find(salary, employeeId);
    if(to_delete== nullptr)
    {
        return;
    }
    root=deleteNode(root,to_delete);
    size_of_tree--;
    min= getMinNode(root);
    max= getMaxNode((root));
}

//the user need to make sure the index is valid before calling this function
//invalid: index<0 or index> size_of_tree
AvlRankTree::Node* AvlRankTree::select(int index) const
{
    Node* node=selectNodeOperation(root,index);
    return node;
}

int AvlRankTree::getHeight() const
{
    return (root==nullptr)?0:root->height;
}

void AvlRankTree::printTree() const
{
    printTreeAux(root);
}

AvlRankTree::Iterator::Iterator(Node* node, Node* last):node(node),last(last){}

AvlRankTree::Iterator& AvlRankTree::Iterator::operator=(const Iterator& other_iter)
{
    if(*this == other_iter)
    {
        return *this;
    }
    last = other_iter.last;
    node = other_iter.node;
    return *this;
}

AvlRankTree::Iterator::Iterator(const Iterator& other_iter):node(other_iter.node),
                                            last(other_iter.last){}

AvlRankTree::Node* AvlRankTree::Iterator::operator*() //may return null-the user 
{                                                     //need to be careful
    return node;
}

bool AvlRankTree::Iterator::operator==(const Iterator& iter) const
{
    if(node==iter.node)
    {
        return true;
    }
    return false;
}
bool AvlRankTree::Iterator::operator!=(const Iterator& iter) const
{
    return !(*this == iter);
}

AvlRankTree::Iterator& AvlRankTree::Iterator::operator++()
{
    if(node == last)
    {
        node = nullptr;
    }
    else
    {
        if(node->right!= nullptr)
        {
            node=node->right;
            while(node->left!= nullptr)
            {
                node=node->left;
            }
        }
        else
        {
            Node* tempNode=node;
            node=node->father;
            while((node->father!= nullptr) && (node->right==tempNode)) //came from right son
            {
                tempNode=node;
                node=node->father;
            }
        }

    }
    return *this;
}

AvlRankTree::Iterator AvlRankTree::begin() const
{
    return Iterator(this->min, this->max);
}
AvlRankTree::Iterator AvlRankTree::end() const
{
    return Iterator(nullptr, nullptr);
}

//will merge 2 tree to one in O(n) - on the user to delete the source's trees
AvlRankTree::AvlRankTree(const AvlRankTree& firstTree, const AvlRankTree& secondTree):
        size_of_tree(0),root(nullptr),max(nullptr),min(nullptr)
{
    Iterator firstIter=firstTree.begin();
    Iterator secondIter=secondTree.begin();
    int size=firstTree.size_of_tree+secondTree.size_of_tree;
    Node** sortedArr = new Node*[size]; //will hold copies!!
    int index=0;
   // int realSize=0;
    while(firstIter!=firstTree.end() && secondIter!=secondTree.end()) //merge
    {
        if( ((*firstIter)->salary) < ((*secondIter)->salary) )
        {
            Node* copy=createNode(((*firstIter)->salary), (*firstIter)->employeeId,
                (*firstIter)->employeeCompanyId, (*firstIter)->employeeGrade);
            //copy only the fields that arent dependent in the sub tree
            
            sortedArr[index]=copy;
            ++firstIter;
        }
        else if(((*firstIter)->salary) > ((*secondIter)->salary))
        {
            Node* copy=createNode(((*secondIter)->salary), (*secondIter)->employeeId,
                (*secondIter)->employeeCompanyId, (*secondIter)->employeeGrade);
            //copy only the fields that arent dependent in the sub tree
            
            sortedArr[index]=copy;
            ++secondIter;
        }
        else //if( ((*firstIter)->salary) == ((*secondIter)->salary) )
        {
            if (((*firstIter)->employeeId) <= ((*secondIter)->employeeId)) //maybe just <
            {
                Node* copy = createNode(((*firstIter)->salary), (*firstIter)->employeeId,
                    (*firstIter)->employeeCompanyId, (*firstIter)->employeeGrade);
                //copy only the fields that arent dependent in the sub tree

                sortedArr[index] = copy;
                ++firstIter;
            }
            else
            {
                Node* copy = createNode(((*secondIter)->salary), (*secondIter)->employeeId,
                    (*secondIter)->employeeCompanyId, (*secondIter)->employeeGrade);
                //copy only the fields that arent dependent in the sub tree

                sortedArr[index] = copy;
                ++secondIter;

            }
        }

        index++; 
    }

    while(firstIter != firstTree.end())
    {
        Node* copy= createNode(((*firstIter)->salary), (*firstIter)->employeeId,
            (*firstIter)->employeeCompanyId, (*firstIter)->employeeGrade);
       
        sortedArr[index]=copy;
        ++firstIter;
        index++;
    }

    while(secondIter != secondTree.end()) 
    {
        Node* copy= createNode(((*secondIter)->salary), (*secondIter)->employeeId,
            (*secondIter)->employeeCompanyId, (*secondIter)->employeeGrade);
        
        sortedArr[index]=copy;
        ++secondIter;
        index++;
    }
    //now we have a sorted array but it may contain duplicated levels
    //- we will merge same leves's data. -O(num of levels)
    Node** sortedUniqeLevelsArr = new Node*[size]; //will hold copies!!
    int j=0; //index at sortedArr
    int k=0; //index at sortedUniqeLevelsArr
    while(j<size)
    {
        if(j<size-1)
        {
            if( ((sortedArr[j])->salary) == ((sortedArr[j+1])->salary) &&
                ((sortedArr[j])->employeeId) == ((sortedArr[j + 1])->employeeId))
                
            {
                Node* copy=createNode((sortedArr[j])->salary, (sortedArr[j])->employeeId
                , (sortedArr[j])->employeeCompanyId, (sortedArr[j])->employeeGrade);
               
                sortedUniqeLevelsArr[k]=copy;
                j+=2; //skip the same level
            }
            else
            {
                Node* copy=createNode((sortedArr[j])->salary, (sortedArr[j])->employeeId
                    , (sortedArr[j])->employeeCompanyId, (sortedArr[j])->employeeGrade);
                
                sortedUniqeLevelsArr[k]=copy;
                j+=1;
            }
            k++;
        }
        else if(j==size-1) //if j>size-1 ->we copied everyones
        { //if we are the prev level was different (because otherwise j=size)
            Node* copy= createNode((sortedArr[j])->salary, (sortedArr[j])->employeeId
                , (sortedArr[j])->employeeCompanyId, (sortedArr[j])->employeeGrade);
            
            sortedUniqeLevelsArr[k]=copy; 
            k++;
            j++;         
        }
    }
    //k hold the true size of the array
    //delete sortedArr - we dont need it anymore
    for(int i=0;i<size;i++) //destroy the copies we made
    {
        delete (sortedArr[i]);
    }
    delete[] sortedArr;
    convertSortedArrayToTree(sortedUniqeLevelsArr,k); //make copies for itself
    //delete sortedUniqeLevelsArr
    for(int i=0;i<k;i++) //destroy the copies we made
    {
        
        delete (sortedUniqeLevelsArr[i]);
    }
    delete[] sortedUniqeLevelsArr;
}

//on the user: "this" is empty (call empty before using)
void AvlRankTree::fillFromTree(const AvlRankTree& src)
{
    Node** sortedSrcArr = new Node*[src.size_of_tree];
    int index=0;
    AvlRankTree::Iterator itr=src.begin();
    while(itr!=src.end())
    {
        Node* copy=createNode(((*itr)->salary), ((*itr)->employeeId),
            ((*itr)->employeeCompanyId) , ((*itr)->employeeGrade));
        //copy only the fields that arent dependent in the sub tree
        
        sortedSrcArr[index]=copy;
        ++itr;
        index++;
    }
    this->convertSortedArrayToTree(sortedSrcArr,src.size_of_tree); //make copies for itself
    for(int i=0;i<src.size_of_tree;i++) //destroy the copies we made
    {
        delete (sortedSrcArr[i]);
    }
    delete[] sortedSrcArr;
    this->size_of_tree=src.size_of_tree;
    max=getMaxNode(this->root);
    min=getMinNode(this->root);
}

//we call this function knowing first and second are in the tree- isn't empty
AvlRankTree::Node* AvlRankTree::getCommonFather(Node* first,Node* second,Node* origin)//o(logn)
{
//    if(origin== nullptr)
//    {
//        return nullptr;
//    }
    //if both first and second are smaller than the origin ,
    // the common father is at the left size
    if(((origin->salary > first->salary) ||
        ((origin->salary == first->salary) && (origin->employeeId > first->employeeId)) )
        &&
        ((origin->salary > second-> salary) ||
        ((origin->salary == second->salary) && (origin->employeeId > second->employeeId))) )
    {
        if(origin->left== nullptr)
        {
            return origin;
        }
        else
        {
            return getCommonFather(first,second,origin->left);
        }

    }
    //if both first and second are bigger than the origin ,
    // the common father is at the right size
    if (((origin->salary < first->salary) ||
        ((origin->salary == first->salary) && (origin->employeeId < first->employeeId)))
        &&
        ((origin->salary < second->salary) ||
            ((origin->salary == second->salary) && (origin->employeeId < second->employeeId))))
        
    {
        if(origin->right== nullptr)
        {
            return origin;
        }
        else
        {
            return getCommonFather(first,second,origin->right);
        }
    }
    return origin;
}

////error: if score<0 or score>scale and num of people between bounds is 0
////or if the bounds exist but there are no people between them (regardless the score)
////return (num of people with score between bounds / num of people between bounds)
//double AvlRankTree::getPrecentOfPlayersWithScoreBetweenBounds(int score,int lowLevel,int highLevel,
//                       int* error, int numPeopleAtZero, int numPeopleAtZeroWithScore)
//{
//    bool score_invalid=false;
//    if (score <= 0 || score > this->scale)
//    {
//        score_invalid=true;
//    }
//    bool low_high_same=false;
//    if(lowLevel==highLevel)
//    {
//        low_high_same=true;
//    }
//    int numOfPeopleWithScore =(lowLevel<=0)?numPeopleAtZeroWithScore:0;
//    int numOfPeopleBetweenBounds =(lowLevel<=0)?numPeopleAtZero:0;
//    Node *low = findNode(this->root, lowLevel);
//    Node *high = findNode(this->root, highLevel);
//    int numsOfFakeLevels = 0; //in order to reduce them from the count
//    bool removeLow=false;
//    bool removeHigh=false;
//    if (low == nullptr) {
//       // if(score_invalid==true)
//        //{
//            this->insert(lowLevel, 0); //will add a fake level with score //may throw bad alloc
//                                      //we use 0 because score 0 is invalid score
//            low = findNode(this->root, lowLevel); //low point to the fake level
//            numsOfFakeLevels++;
//            removeLow=true;
//       // }
////        else
////        {
////            this->insert(lowLevel, score); //will add a fake level with score //may throw bad alloc
////            low = findNode(this->root, lowLevel); //low point to the fake level
////            numsOfFakeLevels++;
////            removeLow=true;
////        }
//    }
//    if (high == nullptr) {
//        if( low_high_same==true) //no need to insert two- will make problems because insert  first remove
//        {
//            high=low;
//        }
//        else
//        {
//         //   if(score_invalid==true)
//           // {
//                this->insert(highLevel, 0); //will add a fake level with score
//                high = findNode(this->root, highLevel); //high point to the fake level
//                numsOfFakeLevels++;
//                removeHigh=true;
//            //}
////            else
////            {
////                this->insert(highLevel, score); //will add a fake level with score
////                high = findNode(this->root, highLevel); //high point to the fake level
////                numsOfFakeLevels++;
////                removeHigh=true;
////            }
//        }
//
//    }
//    if (lowLevel == highLevel) {
//        if (score <= 0 || score > this->scale) //numOfPeopleWithScore==0
//        {
//            numOfPeopleBetweenBounds += low->numPeopleAtLevel - numsOfFakeLevels;
//            *error = (numOfPeopleBetweenBounds == 0) ? -1 : 0;
//            if(removeLow==true)
//            {
//                this->remove(lowLevel,score);
//            }
//            if(removeHigh==true)
//            {
//                this->remove(highLevel,score);
//            }
//            return 0;
//        }
//        else //score is a valid num
//        {
//            numOfPeopleBetweenBounds += low->numPeopleAtLevel - numsOfFakeLevels;
//            numOfPeopleWithScore += low->groupScoreArr[score]; // - numsOfFakeLevels;
//            if (numOfPeopleBetweenBounds == 0)
//            {
//                if(removeLow==true)
//                {
//                    this->remove(lowLevel,score);
//                }
//                if(removeHigh==true)
//                {
//                    this->remove(highLevel,score);
//                }
//                *error = -1;
//                return 0;
//            }
//            else
//            {
//                if(removeLow==true)
//                {
//                    this->remove(lowLevel,score);
//                }
//                if(removeHigh==true)
//                {
//                    this->remove(highLevel,score);
//                }
//                *error=0;
//                return (((double) numOfPeopleWithScore) / ((double) numOfPeopleBetweenBounds))*100;
//
//            }
//        }
//    } else //low and high are not the same node
//    {
//        Node* commonFather = getCommonFather(low, high, root);
//        int numOfPlayersAtCommonFatherSubTree=commonFather->numPeopleAtSubTree;
//        int numOfPlayersUnderLow=(low->left== nullptr)?0:low->left->numPeopleAtSubTree;
//        int numOfPlayersAboveHigh=(high->right== nullptr)?0:high->right->numPeopleAtSubTree;
//        numOfPeopleBetweenBounds+=numOfPlayersAtCommonFatherSubTree-numOfPlayersAboveHigh
//                -numOfPlayersUnderLow-numsOfFakeLevels;
//        if (score <= 0 || score > this->scale) //numOfPeopleWithScore==0 (score is invalid)
//        {
//            if(removeLow==true)
//            {
//                this->remove(lowLevel,score);
//            }
//            if(removeHigh==true)
//            {
//                this->remove(highLevel,score);
//            }
//                *error = (numOfPeopleBetweenBounds == 0) ? -1 : 0;
//                return 0;
//        }
//        else //score is valid
//        {
//            int numWithScoreAtCommonFatherSubTree=commonFather->subTreeGroupScoreArr[score];
//            int numWithScoreUnderLow=(low->left== nullptr)?0:low->left->subTreeGroupScoreArr[score];
//            int numWithScoreAboveHigh=(high->right== nullptr)?0:high->right->subTreeGroupScoreArr[score];
//            numOfPeopleWithScore+=numWithScoreAtCommonFatherSubTree-numWithScoreAboveHigh
//                    -numWithScoreUnderLow; //-numsOfFakeLevels;
//            if(numOfPeopleBetweenBounds==0)
//            {
//                if(removeLow==true)
//                {
//                    this->remove(lowLevel,score);
//                }
//                if(removeHigh==true)
//                {
//                    this->remove(highLevel,score);
//                }
//                *error=-1;
//                return 0;
//            }
//            else
//            {
//                if(removeLow==true)
//                {
//                    this->remove(lowLevel,score);
//                }
//                if(removeHigh==true)
//                {
//                    this->remove(highLevel,score);
//                }
//                *error=0;
//                return 100*(((double)numOfPeopleWithScore)/((double)numOfPeopleBetweenBounds));
//            }
//        }
//    }
//}

//helper func for getPrecentOfPlayersWithScoreBetweenBounds-sum from root until HighLevel
void AvlRankTree::sumUntilReachHighLevel(Node* start,int highLevel, long long* sum_grades,int* in_range)
{
    if(start->salary==highLevel) //we already counted before calling this function
    {
        *sum_grades=0;
        *in_range=0;
        return;
    }
    else
    {
        int in_range_temp=0;
        int sum_grades_temp=0;
        Node* curr=start;
        while(curr!= nullptr) // like while(1)- we stop before reaching null
        {
            
            if(curr->salary > highLevel || (curr->salary == highLevel&& curr->employeeId >0 ))
            {               
                curr=curr->left; //exist because the path to the highLevel exist
            }
            else if(curr-> salary == highLevel && curr->employeeId==0) //found
            {
                in_range_temp+=(curr->left== nullptr)?0:curr->left->numPeopleAtSubTree;
                sum_grades_temp +=(curr->left== nullptr)?0:curr->left->sumOfGradesInSubTree;
               
                *sum_grades = sum_grades_temp;
                *in_range=in_range_temp;
                return;
            }
            else if (curr->salary < highLevel)
            {
                if (curr != start)
                {
                    in_range_temp += (curr->left == nullptr) ? 0 : curr->left->numPeopleAtSubTree;
                    sum_grades_temp += (curr->left == nullptr) ? 0 : curr->left->sumOfGradesInSubTree;
                    in_range_temp += 1;
                    sum_grades_temp+=curr->employeeGrade;
                   
                }
                curr = curr->right; //exist because the path to the highLevel exist
            }
        }
    }
}

//helper func for getPrecentOfPlayersWithScoreBetweenBounds-sum from root until lowLevel
void AvlRankTree::sumUntilReachLowLevel(Node* start,int lowLevel, long long* sum_grades,int* in_range)
{
    if(start->salary==lowLevel && start->employeeId==0) //we already counted before calling this function
    {
        *sum_grades=0;
        *in_range=0;
        return;
    }
    else
    {
        int in_range_temp=0;
        int sum_grade_temp=0;
        Node* curr=start;
        while(curr != nullptr) // like while(1)- we stop before reaching null
        {
            if(curr->salary > lowLevel || (  (curr->salary == lowLevel) && (curr->employeeId > 0)))
            {
                if(curr != start)
                {
                    in_range_temp+=(curr->right== nullptr)?0:curr->right->numPeopleAtSubTree;
                    sum_grade_temp += (curr->right == nullptr) ? 0 : curr->right->sumOfGradesInSubTree;
                    sum_grade_temp += curr->employeeGrade;
                    in_range_temp += 1;
                }
                curr=curr->left; //exist because the path to the lowLevel exist
            }
            
            else if(curr-> salary == lowLevel && curr->employeeId==0) //found
            {
                //in_range_temp+=(curr->right== nullptr)?0:curr->right->numPeopleAtSubTree +1; //maybe without +1
                //sum_grade_temp += (curr->right == nullptr) ? 0 : curr->right->sumOfGradesInSubTree;
                
                *sum_grades= sum_grade_temp;
                *in_range=in_range_temp;
                return;
            }
            else if (curr->salary < lowLevel)
            {
                curr = curr->right; //exist because the path to the highLevel exist
                //nothing to sum
            }
        }
    }
}

double AvlRankTree::getAverageBumpGradeBetweenSalaryByGroup(int lowLevel,int highLevel,int* error,
                    int numPeopleAtZero,long long sumGradeZero)
{
    
    long long sum_grades_in_range = 0;
    int num_people_in_range=0;
    int num_of_fake_nodes = 0;

    if (lowLevel ==0 && highLevel== lowLevel ) //only zero
    {
        double val = 0;
        if (numPeopleAtZero == 0)
        {
            *error = -1;
            val = 0;
            return val;
        }
        val = ((double(sumGradeZero)) / (double(numPeopleAtZero)));
        return val;
    }
    else
    {
        Node* low;
        Node* high;

        insert(lowLevel, 0,0,0); //we insert a fake employee, with salary=lowLevel, id=0 
        //(invalid, therfore smallest posible), with grade=0
        insert(highLevel + 1, 0, 0, 0); //we insert a fake employee, with salary=highLevel+1, id=0 
        //(invalid, therfore smallest posible for salary), with grade=0
        low = findNode(root, lowLevel ,0);
        high = findNode(root, highLevel + 1, 0);

        Node* commonParent = getCommonFather(low, high, root);
        //high and low are different levels so the common father can't be null 

        int in_range_high = 0;
        long long sum_grades_in_range_high = 0;
        sumUntilReachHighLevel(commonParent, highLevel+1, &sum_grades_in_range_high, &in_range_high);
        int in_range_low = 0;
        long long sum_grades_in_range_low = 0;
        sumUntilReachLowLevel(commonParent, lowLevel, &sum_grades_in_range_low, &in_range_low);
        num_people_in_range = num_people_in_range + in_range_high + in_range_low;
        sum_grades_in_range = sum_grades_in_range + sum_grades_in_range_high + sum_grades_in_range_low;
        if (commonParent != nullptr&& commonParent->employeeId!=0)
        {
            num_people_in_range++;
            sum_grades_in_range += commonParent->employeeGrade;
        }
        
        remove(highLevel+1, 0);
        remove(lowLevel, 0);
            
        //root = deleteNode(root, level_zero);
        *error = 0;
        if (lowLevel == 0)
        {
            sum_grades_in_range += sumGradeZero;
            num_people_in_range += numPeopleAtZero;
        }
        if (num_people_in_range == 0)
        {
            *error = -1;
        }
        double val = ((double(sum_grades_in_range)) / (double(num_people_in_range)));
        return val;
    }
}

int AvlRankTree::getTotalNumPeopleAtTree() const
{
    return (root== nullptr)?0:root->numPeopleAtSubTree;
}

int AvlRankTree::getSumOfHighestMPlayerLevel(int sum,int m,Node* node)
{
    if (m <= 0||node==nullptr)
    {
        return sum;
    }
    if (node->right != nullptr)
    {
        if (node->right->numPeopleAtSubTree + 1 == m)
        {
            sum += node->right->sumOfGradesInSubTree + node->employeeGrade;
            return sum;
        }
        else if (node->right->numPeopleAtSubTree >= m)
        {
            return getSumOfHighestMPlayerLevel(sum, m, node->right);
        }
        else
        {
            m -= (node->right->numPeopleAtSubTree + 1/*for root*/);
            sum += node->right->sumOfGradesInSubTree + node->employeeGrade;
            return getSumOfHighestMPlayerLevel(sum, m, node->left);
        }
    }
    else
    {

        sum += node->employeeGrade;
        m = m - 1/*for root*/;
        return getSumOfHighestMPlayerLevel(sum, m, node->left); //node->left surly exist

    }


    ////my
    //if(m!=0 && node!=nullptr && node->right!= nullptr)
    //{
    //    
    //    if (node->right->numPeopleAtSubTree +1 == m )
    //    {
    //        sum+=(node->sumOfGradesInSubTree - node->left->numPeopleAtSubTree);
    //        return sum;
    //    }

    //    if(node->right->numPeopleAtSubTree>=m)
    //    {
    //        
    //        return getSumOfHighestMPlayerLevel(sum ,m,node->right);
    //    }
    //    else
    //    {  //take all the right tree and see how much you can take from node
    //        sum += node->employeeGrade;
    //        if (node->right != nullptr) 
    //        {
    //            sum += node->right->sumOfGradesInSubTree;
    //            m = m - node->right->numPeopleAtSubTree - 1; //maybe to add -1
    //        }
    //        
    //        return getSumOfHighestMPlayerLevel(sum, m, node->left); //node->left exist
    //                                                         //because m>= num people at three
    //        
    //    }
    //}
    //else
    //{
    //    
    //    sum+=node->employeeGrade;
    //    m=m-node->numPeopleAtSubTree;
    //    return getSumOfHighestMPlayerLevel(sum,m,node->left); //node->left surly exist
    //    
    //}
}

AvlRankTree::Node* AvlRankTree::getRoot() const
{
    return this->root;
}







