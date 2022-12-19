#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class Node {
    public:
        Elem* newN;
        Node* left;
        Node* right;
        int Freq;
        int Age;
        bool flag_RW;
        bool hash;
    public:
        Node() {
            this->newN = NULL;
            this->Freq = 0;
        }

        Node(Elem* Elm) {
            this->newN = Elm;
            this->left = NULL;
            this->right = NULL;
            this->Freq = 1;
            this->Age = 0;
            this->flag_RW = false;
        }
};

class Heap: public Node {
    public:
        Node* arr;
        Node* PrimeArray;
        int size;
        int lpos;
        int minOrmax;
        int idx_prime;       
    public:
        Heap(){}
        Heap(int i){ 
            this->lpos = -1;
            this->minOrmax = i;
            this->idx_prime = -1;
        }
        ~Heap(){delete[] arr; delete[] PrimeArray;}
        void insertHeap(Node* N){
            this->lpos++;
            this->arr[lpos] = *N;
            reheapUp(N, lpos);
        }
        
        void reheapUp(Node* N, int pos){
            int parent = (pos-1)/2;
            if(parent >=0){
                if((this->arr[pos].Freq > this->arr[parent].Freq && this->minOrmax == 1)
                ||(this->arr[pos].Freq < this->arr[parent].Freq && this->minOrmax == 2)) {
                    Node temp = this->arr[pos];
                    this->arr[pos] = this->arr[parent];
                    this->arr[parent] = temp;
                    reheapUp(N, parent);
                }
            }
        }
        void reheapDown(Node* arr, int pos, int lpos) {
            int leftChild = pos*2 + 1;
            int rightChild = pos*2 + 2;
            int largeChild, leastChild;
            if(leftChild <= this->lpos){
                if(this->minOrmax == 1){
                    if(rightChild <= lpos && (this->arr[rightChild].Freq >= this->arr[leftChild].Freq))
                        largeChild = rightChild;
                    else 
                        largeChild = leftChild;
                    if (this->arr[largeChild].Freq >= this->arr[pos].Freq) {
                        Node temp = this->arr[largeChild];
                        this->arr[largeChild] = this->arr[pos];
                        this->arr[pos] = temp;
                        reheapDown(this->arr, largeChild, lpos);
                    }
                }
                else if(this->minOrmax == 2){
                    if (rightChild <= lpos && (this->arr[rightChild].Freq < this->arr[leftChild].Freq))
                        leastChild = rightChild;
                    else 
                        leastChild = leftChild;
                    if (this->arr[leastChild].Freq <= this->arr[pos].Freq) {
                        Node temp = this->arr[leastChild];
                        this->arr[leastChild] = this->arr[pos];
                        this->arr[pos] = temp;
                        reheapDown(this->arr, leastChild, lpos);
                    }
                }
            }
        }
        void deleteHeap(Node* arr, Node* dele) {
            for(int i = 0; i <= lpos; i++){
                if(arr[i].newN->addr == dele->newN->addr){
                    Node temp = arr[i];
                    arr[i] = arr[this->lpos];
                    arr[this->lpos] = temp;
                    lpos--;
                    if(i > 0 && i < lpos) {
                        if(i >= this->size/2)
                            reheapUp(&arr[i], i);

                        if(arr[(i-1)/2].Freq < arr[i].Freq && this->minOrmax == 1)
                            reheapUp(&arr[i], i);
                            
                        else if((arr[2*i+1].Freq >= arr[i].Freq 
                        || arr[2*i+2].Freq >=arr[i].Freq) && this->minOrmax == 1)
                            reheapDown(&arr[i], i, lpos);
                            
                        if(arr[(i-1)/2].Freq > arr[i].Freq && this->minOrmax == 2)
                            reheapDown(&arr[i], i, lpos);
                            
                        else if((arr[2*i+1].Freq <= arr[i].Freq 
                        || arr[2*i+2].Freq <= arr[i].Freq) && this->minOrmax == 2)
                            reheapDown(&arr[i], i, lpos);
                        }
                    else if(i == 0) 
                        reheapDown(&arr[i], i , lpos);
                        
                    else if(i == lpos) 
                        reheapUp(&arr[i], i);   
                }
            }
        }

        int get_idx(Node* node){
            for(int i = 0; i <= this->idx_prime; i++){
                if(this->PrimeArray[i].newN->addr == node->newN->addr)
                    return i;
            }
        }
        void insertPrime(Node* node, int idx){
            if(this->idx_prime == -1){
                this->PrimeArray[0] = *node;
                this->idx_prime++;
            }
            else if(this->idx_prime < MAXSIZE-1)
                this->PrimeArray[++idx_prime] = *node; 
            else 
                this->PrimeArray[idx] = *node;
            
        }

        void Hashtable() {
            Node* hash = new Node[MAXSIZE];
            for(int i = 0; i < this->size; i++) {
                hash[i].hash = false;
            }
            for(int i = 0; i <= this->lpos; i++) {
                int modulo = this->PrimeArray[i].newN->addr % this->size;
                
                if(modulo > this->lpos && hash[modulo].hash == false){
                    for(int j = 0; j < this->size ; j++){
                        if(hash[j].hash == false){
                            hash[j] = this->PrimeArray[i];
                            hash[j].hash = true;
                            break;
                        }
                    }
                }
                if(hash[modulo].hash == false) {
                    hash[modulo] = this->PrimeArray[i];
                    hash[modulo].hash = true;
                }
                else {
                    int k = modulo;
                    bool flag = false;
                    while(k <= this->lpos){
                        if(hash[k].hash == false){
                            hash[k] = this->PrimeArray[i];
                            hash[k].hash = true;
                            flag = true;
                            break;
                        }
                        k++;
                    }
                    if(k > this->lpos && flag == false){
                        for(int j = 0; j <= this->lpos ; j++){
                            if(hash[j].hash == false){
                                hash[j] = this->PrimeArray[i];
                                hash[j].hash = true;
                                break;
                            }
                        }
                    }
                }
            }
            for(int i = 0; i <= this->lpos; i++)
                hash[i].newN->print();
        } 
};

class ReplacementPolicy {
    //TODO
    public:
        int flagP;
        int flagML;
};

class MFU : public ReplacementPolicy, public Heap {
    //TODO
    public:
        Heap* newH;
    public:
        MFU() {
            this->newH = new Heap(1);
            this->flagP = 0;
            this->flagML = 0;
        }
        void printMFU() {
            for(int i = this->newH->lpos; i >= 0; i--) 
                this->newH->arr[i].newN->print();   
        }
};

class LFU : public ReplacementPolicy, public Heap {
    //TODO
    public:
        Heap* newH;
    public:
        LFU() {
            this->newH = new Heap(2);
            this->flagP = 1;
            this->flagML = 1;
        }
        void printLFU() {
            for(int i = 0 ; i <= this->newH->lpos; i++)
                this->newH->arr[i].newN->print();
        }
};

class MFRU: public ReplacementPolicy, public Heap {
    //TODO
    public:
        Node* arr;
        int curr;
        int index;
    public:
        MFRU() {
            this->curr = -1;
            this->flagP = 2;
            this->flagML = 2;
            this->index = -1;
            this->idx_prime = -1;
        }
        void insertMFRU(Node* N) {
            if(this->curr == -1){
                this->arr[0] = *N;
                this->arr[0].Age = 0;
                this->curr++;
                this->index++;
                return;
            }
            this->curr++;
            this->index++;
            this->arr[curr] = *N;
            this->arr[curr].Age = index;
            this->selectionSort();
        }
        void deleteMFRU(){
            for(int i = 0; i < this->curr; i++) {
                this->arr[i] = this->arr[i+1];
            }
            this->curr--;
            this->index--;
            for(int i = 0; i <= this->curr; i++)
                this->arr[i].Age--;
        }

        void incrementFeq(Node* node){
            for(int i = 0; i <= this->curr; i++){
                if(this->arr[i].newN->addr == node->newN->addr){
                    this->arr[i].Freq++;
                    index += 1;
                    this->arr[i].Age = index; 
                }
            }
            this->selectionSort();
        }
        void selectionSort(){
            for(int i = 0; i < this->curr; i++){
                for(int j = i+1; j <= this->curr; j++){
                    if(this->arr[j].Freq > this->arr[i].Freq){
                        Node temp = this->arr[j];
                        this->arr[j] = this->arr[i];
                        this->arr[i] = temp;
                    }
                }
            }
            for(int i = 0; i < this->curr; i++){
                for(int j = i+1; j <= this->curr; j++){
                    if(this->arr[j].Freq >= this->arr[i].Freq 
                    && this->arr[j].Age > this->arr[i].Age){
                        Node temp = this->arr[j];
                        this->arr[j] = this->arr[i];
                        this->arr[i] = temp;
                    }
                }
            }
        }

        void printMFRU(){
            this->selectionSort();
            for(int i = 0; i <= this->curr; i++) 
                this->arr[i].newN->print();
        }

};

class LFRU: public ReplacementPolicy, public Heap {
    //TODO
    public:
        Node* arr;
        int curr;
        int index;
    public:
        LFRU(){
            this->curr = -1;
            this->flagP = 3;
            this->flagML = 3;
            this->index = -1;
            this->idx_prime = -1;
        }
        void insertLFRU(Node* N){
            if(this->curr == -1){
                this->arr[0] = *N;
                this->arr[0].Age = 0;
                this->curr++;
                this->index++;
                return;
            }
            this->curr++;
            this->index++;
            this->arr[curr] = *N;
            this->arr[curr].Age = index;
            this->selectionSort();
        }
        void incrementFeq(Node* node){
            for(int i = 0; i <= this->curr; i++){
                if(this->arr[i].newN->addr == node->newN->addr){
                    this->arr[i].Freq++;
                    index += 1;
                    this->arr[i].Age = index; 
                }
            }
            this->selectionSort();
        }
        void deleteLFRU(){
            for(int i = 0; i < this->curr; i++) {
                this->arr[i] = this->arr[i+1];
            }
            this->curr--;
            this->index--;
            for(int i = 0; i <= this->curr; i++)
                this->arr[i].Age--;
        }
        void selectionSort(){
            for(int i = 0; i < this->curr; i++){
                for(int j = i; j <= this->curr; j++){
                    if(this->arr[j].Freq < this->arr[i].Freq 
                    || (this->arr[j].Freq == this->arr[i].Freq
                    && this->arr[j].Age < this->arr[i].Age)){
                        Node temp = this->arr[j];
                        this->arr[j] = this->arr[i];
                        this->arr[i] = temp;
                    }
                }
            }
        }
        void printLFRU(){
            this->selectionSort();
            for(int i = 0; i < this->size; i++)
                this->arr[i].newN->print();    
        }
};

class SearchEngine {
    //TODO
};

class BST : public SearchEngine {
    //TODO
    public:
        Node* node;
        BST* left, *right;
        int p;
    public:
        BST(){
            this->p = 0;
        }

        void insertBST(Node*& root, Node* node) {
            this->p++;
            if(!root) {
                root = node;
                return;
            }
            Node* prev = NULL;
            Node* temp = root;
            while(temp) {
                if(temp->newN->addr > node->newN->addr){
                    prev = temp;
                    temp = temp->left;
                }
                else if(temp->newN->addr <= node->newN->addr){
                    prev = temp;
                    temp= temp->right;
                }
            }
            if(prev->newN->addr > node->newN->addr)
                prev->left = node;
            else 
                prev->right = node; 
        }

        Node* deleteBST(Node* root, int addr){
            if(root == NULL) 
                return root;
            if(root->newN->addr > addr){
                root->left = deleteBST(root->left, addr);
                return root;
            }
            else if(root->newN->addr < addr){
                root->right = deleteBST(root->right, addr);
                return root;
            }
            if(root->left == NULL){
                Node* temp = root->right;
                free(root);
                return temp;
            }
            else if(root->right == NULL){
                Node* temp = root->left;
                free(root);
                return temp;
            }
            else {
                Node* succParent  = root;
                Node* succ = root->right;
                
                while(succ->left != NULL){
                    succParent = succ;
                    succ = succ->left;
                }
                if(succParent != root)
                    succParent->left = succ->right;
                else
                    succParent->right = succ->right;

                root->newN->addr = succ->newN->addr;
                free(succ);
                return root;
            }
        }

        Node* searchNode(Node* root, int addr) {
            Node* temp = root;
            if(root == NULL) return NULL;
            else {
                while(temp) {
                    if(temp->newN->addr > addr)
                        temp = temp->left;
                    else if (temp->newN->addr < addr)
                        temp = temp->right;
                    else 
                        return temp;
                }
            }
        }

        void printPreorder(Node* ptr) {  
            if (ptr == NULL) return;
            ptr->newN->print();
            printPreorder(ptr->left);
            printPreorder(ptr->right);
        }

        void printInorder(Node* ptr) {       
            if (ptr == NULL) return;
            printInorder(ptr->left);
            ptr->newN->print();
            printInorder(ptr->right);
        }
};

#endif