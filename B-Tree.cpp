#include  <bits/stdc++.h>
using namespace std;

struct BTreeNode
{
	int t;
	BTreeNode **Childs;
	int *keys;
	int n;
	bool leaf;

	BTreeNode(int _t)
    {
	t = _t;
	Childs = new BTreeNode *[2*t];
	keys = new int[2*t-1];
	n = 0;
	leaf = true;
    }

    bool Search(int k)
    {
	int i = 0;
	for(i;i < n;i++)
        if(k <= keys[i])
            break;
	if (keys[i] == k)
		return true;
	if (leaf)
		return false;
	return Childs[i]->Search(k);
    }

    void insertNonfull(int k){
	int i;
	if(leaf)
	{
		for(i=n-1;i >= 0;i--)
		{
		    if(k> keys[i])
                break;
			keys[i+1] = keys[i];
		}

		keys[i+1] = k;
		n++;
	}
	else
	{
		for(i=n-1;i >= 0;i--)
			if(k>keys[i])
                break;
		if (Childs[i+1]->n == 2*t-1)
		{
			splitChild(i+1, Childs[i+1]);
			if (k>keys[i+1])
				i++;
		}
		Childs[i+1]->insertNonfull(k);
	}
}

    void splitChild(int i, BTreeNode* Node){
        BTreeNode *newNode = new BTreeNode(Node->t);
        newNode->n = t-1;
        newNode->leaf = Node->leaf;

        for (int j = 0; j < t-1; j++)
            newNode->keys[j] = Node->keys[j+t];

        if (!Node->leaf)
        {
            for (int j = 0; j<t; j++)
                newNode->Childs[j] = Node->Childs[j+t];
        }
        Node->n = t-1;
        for (int j = n+1; j > i+1; j--)
            Childs[j] = Childs[j-1];
        Childs[i+1] = newNode;
        for (int j = n; j > i; j--)
            keys[j] = keys[j-1];
        keys[i] = Node->keys[t-1];
        n++;
}


};

struct BTree
{
	BTreeNode *root;
	int t;
public:
	BTree(int _t)
	{   t = _t;
	    root = nullptr;
	    }

	bool Search(int k){
	    if(!root)
            return false;
        return root->Search(k);
    }

	void Insert(int k){
	if (!root)
	{
		root = new BTreeNode(t);
		root->keys[0] = k;
		root->n = 1;
		root->leaf = true;
	}
	else if (root->n == 2*t-1)
		{
			BTreeNode *newRoot = new BTreeNode(t);
			newRoot->leaf = false;
			newRoot->Childs[0] = root;
			newRoot->splitChild(0, root);
			int i = 0;
			if (k>newRoot->keys[0])
				i++;
			newRoot->Childs[i]->insertNonfull(k);
			root = newRoot;
		}
    else
			root->insertNonfull(k);
    }
};

struct StackNode{
    int data;
    StackNode* next;
    StackNode(int x=0 ,StackNode* n= nullptr){
        data = x;
        next = n;
        }
};

struct Stack{
    int sz;
    StackNode* top;
    Stack(){
    top = nullptr;
    sz = 0;
    }
    void push(int x){
        top = new StackNode(x,top);
        sz++;
        }
    int pop(){
        int tmp = top->data;
        top = top->next;
        sz--;
        return tmp;
        }
    bool isEmpty(){
        if(top == nullptr)
            return true;
        return false;
        }
};
struct Graph{
    int numVertices;
    Stack* adjency_list;
    Graph(int n){
        numVertices = n;
        adjency_list = new Stack[n];
    }
    void addEdge(int u, int v) {
        adjency_list[u].push(v);
    }

    Graph transpose(){
        Graph rev(numVertices);
        for(int i= 1; i<numVertices ; i++){
            Stack s = this->adjency_list[i];
            while(!s.isEmpty()){
                int tmp = s.pop();
                rev.addEdge(tmp,i);
            }
        }
        return rev;
}

    Stack firstDFS(){
        Stack stk;
        BTree B(3);
        for(int i=1; i<numVertices; i++)
            if(!B.Search(i))
                fillOrder(i,B,stk);
        return stk;
    }

    void fillOrder(int i,BTree &B,Stack &stk ){
        B.Insert(i);
        Stack s = adjency_list[i];
        while(!s.isEmpty()){
            int tmp = s.pop();
            if(!B.Search(tmp))
                fillOrder(tmp,B,stk);
            }
            stk.push(i);}

    void recDFS(int i,BTree &B,Stack &res_k){
        B.Insert(i);
        res_k.push(i);
        Stack s = adjency_list[i];
        while(!s.isEmpty()){
            int tmp = s.pop();
            if(!B.Search(tmp))
                recDFS(tmp,B,res_k);
            }
                }
Stack* SCC(){
   Stack* res = new Stack[numVertices];
    Stack stk;
    stk = this->firstDFS();
    Graph gp = this->transpose();
    BTree b(3);
    int k = 0;
    while(!stk.isEmpty())
    {
        int x = stk.pop();
        if(!b.Search(x))
        {
            Stack res_k;
            gp.recDFS(x,b,res_k);
            res[k]=res_k;
            k++;
        }
    }
    return res;
}
};

int main()
{
    int n;
    cin>>n;
    n++;
    string arr_hash[n];
    string arr[n][n];
    for(int i =0; i<n; i++){
       string x;
       getline(cin,x);
       int k = 0;
       string y = "";
       for(int j = 0; j<x.length(); j++){
            if(x[j] == ':' || x[j] == ' '){
               arr[i][k] = y;
               k++;
               y = "";
               j++;
            }
            y+=x[j];
            if(j == x.length()-1)
                arr[i][k] = y;

       }
    }
    for(int i=1;i<n;i++)
        arr_hash[i-1] = arr[i][0];
    sort(arr_hash,arr_hash+n);
    Graph g(n);
    for(int i=1; i<n;i++)
        for(int j=1;j<n;j++)
            if(arr[i][j] != ""){
                    int orig,dest;
                    int k = 0;
                    while(true){
                        if(arr_hash[k] == arr[i][0]){
                            orig = k;
                            break;}
                        k++;}
                    k = 0;
                    while(true){
                        if(arr_hash[k] == arr[i][j]){
                            dest = k;
                            break;}
                        k++;}
            g.addEdge(orig,dest);
            }

    Stack* scc;
    scc =g.SCC();

    int result[n][n];
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            result[i][j]=n;

    for(int i=0;i<n;i++){
            int k =0;
        while(true){
            if(scc[i].isEmpty()){
                break;
            }
            int x =scc[i].pop();
            result[i][k]=x;
            k++;
        }
    }
    int rows[n];
    for(int i=0;i<n;i++){
        sort(result[i],result[i]+n);
        rows[i]=result[i][0];
    }
    sort(rows,rows+n);
    for(int i=0;i<n;i++)
    {
        if(rows[i]==n)
            break;
        for(int k=0;k<n;k++){
            if(rows[i]==result[k][0]){
                for(int j=0;j<n;j++){
                    if(result[k][j]==n)
                        break;
                        cout<<arr_hash[result[k][j]]<<" ";
                        }
                    }
                }
        cout<<endl;
    }
    return 0;
}
