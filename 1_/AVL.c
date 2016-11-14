#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct AVL_Node
{
    int Key;
    int Height;
    struct AVL_Node* LeftChild;
    struct AVL_Node* RightChild; 
}Node,*Tree;

int Node_Height(Tree tree);
void Maintain_Height(Tree tree);
Node* Node_Create(int key,Node* left,Node* right);
Node* L_L_Rotate(Tree father);
Node* R_R_Rotate(Tree father);
Node* L_R_Rotate(Tree father);
Node* R_L_Rotate(Tree father);
Node* Node_Insert(Tree tree,int key);
void Pre_Search(Tree tree);


int Node_Height(Tree tree)
{
    return (tree==NULL)?0:(tree)->Height;
}

#define max(a,b) ((a)>(b)?(a):(b))

void Maintain_Height(Tree tree)
{
    tree->Height=max(Node_Height(tree->LeftChild),Node_Height(tree->RightChild))+1;
}

#undef max

Node* Node_Create(int key,Node* left,Node* right)
{
    Node* p;
    if((p=((Node*)malloc(sizeof(Node))))==NULL)
    {
        return NULL;
    }
    p->Key=key;
    p->Height=0;
    p->LeftChild=left;
    p->RightChild=right;
    return p;
}

static Node* L_L_Rotate(Tree father)
{
    Tree son;
    son=father->LeftChild;
    father->LeftChild=son->RightChild;
    son->RightChild=father;
    Maintain_Height(father);
    Maintain_Height(son);
    return son;
}

static Node* R_R_Rotate(Tree father)
{
    Tree son;
    son=father->RightChild;
    father->RightChild=son->LeftChild;
    son->LeftChild=father;
    Maintain_Height(father);
    Maintain_Height(son);
    return son;
}

static Node* L_R_Rotate(Tree father)
{
    father->LeftChild=R_R_Rotate(father->LeftChild);
    return L_L_Rotate(father);
}

static Node* R_L_Rotate(Tree father)
{
    father->RightChild=L_L_Rotate(father->RightChild);
    return R_R_Rotate(father);
}

Node* Node_Insert(Tree tree,int key)
{
    if(tree==NULL)
    {
        tree=Node_Create(key,NULL,NULL);
        if(tree==NULL)
        {
            puts("Create Tree Failed!");
            exit(0);
        }
    }
    else
    {
        if(key<tree->Key)
        {
            tree->LeftChild=Node_Insert(tree->LeftChild,key);
            if(Node_Height(tree->LeftChild)-Node_Height(tree->RightChild)==2)
            {             
                if(key<tree->LeftChild->Key)
                    tree=L_L_Rotate(tree);
                else
                    tree=L_R_Rotate(tree);
            }
        }
        else
        {
            if(key>tree->Key)
            {
                tree->RightChild=Node_Insert(tree->RightChild,key);
                if(Node_Height(tree->RightChild)-Node_Height(tree->LeftChild)==2)
                {             
                    if(key>tree->RightChild->Key)
                        tree=R_R_Rotate(tree);
                    else
                        tree=R_L_Rotate(tree);
                }
            }
            else
            {
                printf("Insert %d error:the same key has appeared!\n",key);
            }
        }
    }
    Maintain_Height(tree);
    return tree;
}

void Pre_Search(Tree tree)
{
    if((tree->LeftChild)!=NULL)
        Pre_Search(tree->LeftChild);
    if(tree!=NULL)
        printf("%d %d\n",(tree)->Key,(tree)->Height);
    if((tree->RightChild)!=NULL)
        Pre_Search(tree->RightChild);
    return;
}

/*int main()
{
    Tree root=NULL;
    root=Node_Insert(root,1);
    if(root==NULL)
        return 0;
    root=Node_Insert(root,2);
    root=Node_Insert(root,-1);
    root=Node_Insert(root,-1);
    root=Node_Insert(root,8);
    root=Node_Insert(root,7);
    root=Node_Insert(root,6);
    root=Node_Insert(root,5);
    root=Node_Insert(root,3);
    Pre_Search(root);
    return 0;
}*/