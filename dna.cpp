/*DNA SEQUENCING USING SUFFIX TREES
    PROJECT BY
    S SUBHIKSHAA (19PD36)
    SHANIA JOB (19PD32)*/

#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include<fstream>
using namespace std;

#define MAX_CHILD 256

struct Node            //Structure for node
{
    Node *children[MAX_CHILD];
    Node *suffixLink;

    int start;
    int *end;
    int Index;
};

class SuffixTree          //Suffix Tree is built using this class
{

    char text[100]; //Input string
    Node *activeNode;
    Node *lastNode;

    int aEdge;
    int aLength;
    int rCount;
    int leafEnd;
    int *rootEnd;
    int *splitEnd;
    int len; //Length of input string
    int len1;  //length of substring
public:
    Node *root;
    SuffixTree(char t[100], int s1);
    Node *newNode(int start, int *end);
    int edgeLength(Node *n);
    int walkDown(Node *currNode);
    void extendTree(int pos);
    void printTree(int i, int j);
    void setIndex(Node *n, int labelH);
    void freeTree(Node *root);
    void buildTree();
    int traverseEdge(char *str, int idx, int start, int end);
    int ToCountLeaf(Node *n);
    int cLeaf(Node *n);
    int forSubSeq(Node *n, char* str, int idx);
    void checkSubSeq(char* str);
    int forLCS(Node *n, int labelH, int* maxH,  int* StartIndex);
    void getLCS();

};

SuffixTree :: SuffixTree (char t[100], int s1=0)   //Constructor to initialise suffix tree
{
    strcpy(text,t);
    root=NULL;
    activeNode=NULL;
    lastNode=NULL;

    aEdge=-1;
    aLength=0;
    rCount=0;
    leafEnd=-1;
    rootEnd=NULL;
    splitEnd=NULL;
    len=-1;
    len1=s1;
}

Node* SuffixTree:: newNode(int start, int *end)
{
    Node *node =new Node;
    int i;
    for (i = 0; i < MAX_CHILD; i++)
        node->children[i] = NULL;
    node->suffixLink = root;
    node->start = start;
    node->end = end;
    node->Index = -1;
    return node;
}

int SuffixTree:: edgeLength(Node *n)
{
    if(n == root)
        return 0;
    return *(n->end) - (n->start) + 1;
}

int SuffixTree:: walkDown(Node *currNode)
{

    if (aLength >= edgeLength(currNode))
    {
        aEdge += edgeLength(currNode);
        aLength -= edgeLength(currNode);
        activeNode = currNode;
        return 1;
    }
    return 0;
}

void SuffixTree :: extendTree(int pos)
{
    leafEnd = pos;
    rCount++;
    lastNode = NULL;
    while(rCount > 0)
    {
        if (aLength == 0)
            aEdge = pos;

        if (activeNode->children[text[aEdge]] == NULL)
        {
            //Extension Rule 2 (A new leaf edge gets created)
            activeNode->children[text[aEdge]] =
            newNode(pos, &leafEnd);

            if (lastNode != NULL)
            {
                lastNode->suffixLink = activeNode;
                lastNode = NULL;
            }
        }

        else
        {
            // Get the next node at the end of edge starting with a edge
            Node *next = activeNode->children[text[aEdge]];
            if (walkDown(next))//Do walkdown
            {
                //Start from next node (the new activeNode)
                continue;
            }

            if (text[next->start + aLength] == text[pos])
            {
                if(lastNode != NULL && activeNode != root)
                {
                    lastNode->suffixLink = activeNode;
                    lastNode = NULL;
                }
                aLength++;
                break;
            }
            splitEnd = (int*) malloc(sizeof(int));
            *splitEnd = next->start + aLength - 1;

            //New internal node
            Node *split = newNode(next->start, splitEnd);
            activeNode->children[text[aEdge]] = split;
            //New leaf coming out of new internal node
            split->children[text[pos]] = newNode(pos, &leafEnd);
            next->start += aLength;
            split->children[text[next->start]] = next;

            if (lastNode != NULL)
            {
                //suffixLink of lastNode points to current newly created internal node
                lastNode->suffixLink = split;
            }
            lastNode = split;
        }

        rCount--;
        if (activeNode == root && aLength > 0)
        {
            aLength--;
            aEdge = pos - rCount + 1;
        }
        else if (activeNode != root)
        {
            activeNode = activeNode->suffixLink;
        }
    }
}

void SuffixTree :: printTree(int i, int j)
{
    int k;
    for (k=i; k<=j; k++)
        cout<<text[k];
}

void SuffixTree :: setIndex(Node *n, int labelH)
{
    if (n == NULL)  return;

    if (n->start != -1) //A non-root node
    {
       // print(n->start, *(n->end));
    }
    int leaf = 1;
    int i;
    for (i = 0; i < MAX_CHILD; i++)
    {
        if (n->children[i] != NULL)
        {
            leaf = 0;
            setIndex(n->children[i], labelH + edgeLength(n->children[i]));
        }
    }
    if (leaf == 1)
    {
        n->Index = len - labelH;
    }
}

void SuffixTree :: freeTree(Node *root)
{
    if (root == NULL)
        return;
    int i;
    for (i = 0; i < MAX_CHILD; i++)
    {
        if (root->children[i] != NULL)
        {
            freeTree(root->children[i]);
        }
    }
    if (root->Index == -1)
        free(root->end);
    free(root);
}

void SuffixTree :: buildTree()
{
    len = strlen(text);
    int i;
    int *rootEnd = new int;
    *rootEnd = - 1;

    root = newNode(-1, rootEnd);

    activeNode = root; //First activeNode will be root
    for (i=0; i<len; i++)
        extendTree(i);
    int labelHeight = 0;
    setIndex(root, labelHeight);
}

int SuffixTree :: traverseEdge(char *str, int idx, int start, int end)
{
    int k = 0;
    //Traverse the edge with character by character matching
    for(k=start; k<=end && str[idx] != '\0'; k++, idx++)
    {
        if(text[k] != str[idx])
            return -1;  // mo match
    }
    if(str[idx] == '\0')
        return 1;  // match
    return 0;  // more characters yet to match
}

int SuffixTree :: ToCountLeaf(Node *n)
{
    if(n == NULL)
        return 0;
    if(n->Index > -1)
    {
        cout<<"\nFound at position: "<<n->Index;
        return 1;
    }
    int count = 0;
    int i = 0;
    for (i = 0; i < MAX_CHILD; i++)
    {
        if(n->children[i] != NULL)
        {
            count += ToCountLeaf(n->children[i]);
        }
    }
    return count;
}

int SuffixTree :: cLeaf(Node *n)
{
    if(n == NULL)
        return 0;
    return ToCountLeaf(n);
}

int SuffixTree :: forSubSeq(Node *n, char* str, int idx)
{
    if(n == NULL)
    {
        return -1; // no match
    }
    int res = -1;
    //If node n is not root node, then traverse edge from node n's parent to node n.
    if(n->start != -1)
    {
        res = traverseEdge(str, idx, n->start, *(n->end));
        if(res == -1)  //no match
            return -1;
        if(res == 1) //match
        {
            if(n->Index > -1)
                cout<<"\nsubstring count: 1 and position: "<<n->Index;
            else
                cout<<"\nsubstring count: "<<cLeaf(n);
            return 1;
        }
    }
    //Get the character index to search
    idx = idx + edgeLength(n);
    if(n->children[str[idx]] != NULL)
        return forSubSeq(n->children[str[idx]], str, idx);
    else
        return -1;  // no match
}

void SuffixTree :: checkSubSeq(char* str)
{
    int res = forSubSeq(root, str, 0);
    if(res == 1)
        cout<<"\nPattern "<<str<<" is a Sub-sequence\n";
    else
        cout<<"\nPattern "<<str<<" is NOT a Sub-sequence\n";
}
int SuffixTree :: forLCS(Node *n, int labelH, int* maxH,  int* StartIndex)
{
    if(n == NULL)
    {
        return 0;
    }
    int i=0;
    int ret = -1;
    if(n->Index < 0) //If it is internal node
    {
        for (i = 0; i < MAX_CHILD; i++)
        {
            if(n->children[i] != NULL)
            {
                ret = forLCS(n->children[i], labelH +
                    edgeLength(n->children[i]),
                    maxH, StartIndex);

                if(n->Index == -1)
                    n->Index = ret;
                else if((n->Index == -2 && ret == -3) ||
                    (n->Index == -3 && ret == -2) ||
                    n->Index == -4)
                {
                    n->Index = -4;//Mark node as XY
                    //Keep track of deepest node
                    if(*maxH < labelH)
                    {
                        *maxH = labelH;
                        *StartIndex = *(n->end) -
                            labelH + 1;
                    }
                }
            }
        }
    }
    else if(n->Index > -1 && n->Index < len1)//suffix of X
        return -2;//Mark node as X
    else if(n->Index >= len1)//suffix of Y
        return -3;//Mark node as Y
    return n->Index;
}

void SuffixTree :: getLCS()
{
    int maxH = 0;
    int StartIndex = 0;
    int k;
    forLCS(root, 0, &maxH, &StartIndex);
    cout<<"Longest common sub-sequence ";
    for (k=0; k<maxH; k++)
        cout<<text[k + StartIndex];
    cout<<endl;
    if(k == 0)
        cout<<"No common sub-sequence \n";
    else
         cout<<"Length of longest common sub-sequence "<<maxH<<endl;

}

float time()
{

  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  cout<<"\n\t\tClock : "<<asctime(timeinfo)<<endl<<endl;
}

void back_main()
{
    cout<<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    cout<<"\n\t\t\tDNA SEQUENCING USING SUFFIX TREE \n";
    cout<<"\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    time();

}

int main()
{
    char seq[100],ch,ch1,subseq[200];
    int i=0,cho,sublen,cho1;
    ifstream file;
    back_main();
    cout<<"\n1.Human genome \n";
    cout<<"\n2.Rat genome \n";
    cout<<"\n3.Mouse genome \n";
    cout<<"\n4.Pig genome \n";
    cout<<"\nEnter choice : ";
    cin>>cho;
    switch(cho)
    {
        case 1: file.open("human.txt",ios::in );
                break;
        case 2: file.open("rat.txt",ios::in );
                break;
        case 3: file.open("mouse.txt",ios::in );
                break;
        case 4: file.open("pig.txt",ios::in );
                break;
    }
    while(!file.eof())
    {
        file>>ch;
        seq[i]=ch;
        i++;
    }
    i--;
    seq[i]='$';
    i++;
    seq[i]='\0';
    file.close();

    SuffixTree t1(seq);
    t1.buildTree();
    system("cls");
    back_main();

    do
    {
        if(cho==1)
            cout<<"\n\t\tHUMAN GENOME \n";
        else if(cho==2)
            cout<<"\n\t\tRAT GENOME \n";
        else if(cho==3)
            cout<<"\n\t\tMOUSE GENOME \n";
        else
            cout<<"\n\t\tPIG GENOME \n";

        cout<<"\n1. Checking if a sub-sequence is present in a genome and displays its position\n";
        cout<<"\n2. Find the longest common sequence present in two genomes \n";
        cout<<"\nEnter choice : ";
        cin>>cho1;
        system("cls");
        back_main();

        switch(cho1)
        {
            case 1 :
            {
                cout<<"\nEnter sub-sequence : \n";
                cin>>subseq;
                t1.checkSubSeq(subseq);
                break;
            }
            case 2 :
            {
                cout<<"\nEnter sequence to check longest common sub-sequence with genome  :\n";
                cin>>subseq;
                strcat(subseq,"#");
                sublen=strlen(subseq);
                strcat(subseq,seq);
                cout<<subseq<<endl;
                SuffixTree t2(subseq,sublen);
                t2.buildTree();
                t2.getLCS();
                t2.freeTree(t2.root);
                break;
            }
            default :cout<<"\nWrong input \n";

        }
        cout<<"\nDo you want to perform more checking (y/n) ? : ";
        cin>>ch1;
        cout<<endl;
        system("cls");
        back_main();

    }while(ch1=='y' || ch1=='Y');

    system("cls");
    back_main();
    cout<<"\t\tTHANK YOU \n";
    cout<<"\n\nPROJECT BY S SUBHIKSHAA (19PD36) and SHANIA JOB (19PD32)\n\n";

    t1.freeTree(t1.root);

    return 0;
}
