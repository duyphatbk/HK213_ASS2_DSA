#include "main.h"

Node* root = NULL;
Node* sr = NULL;
BST* newBST = new BST();
MFU* M = new MFU();
LFU* L = new LFU();
MFRU* MF = new MFRU();
LFRU* LF = new LFRU();
bool flag = false;

int real_MAXSIZE;

Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {
    real_MAXSIZE = MAXSIZE;
    if(this->rp->flagML == 0){
        M->newH->arr = new Node[real_MAXSIZE];
        M->newH->PrimeArray = new Node[real_MAXSIZE];
        M->newH->size = real_MAXSIZE;
    }
    else if (this->rp->flagML == 1){
        L->newH->arr = new Node[real_MAXSIZE];
        L->newH->PrimeArray = new Node[real_MAXSIZE];
        L->newH->size = real_MAXSIZE;
    }
    else if (this->rp->flagML == 2){
        MF->arr = new Node[real_MAXSIZE];
        MF->PrimeArray = new Node[real_MAXSIZE];
        MF->size = real_MAXSIZE;
    }
    else {
        LF->arr = new Node[real_MAXSIZE];
        LF->PrimeArray = new Node[real_MAXSIZE];
        LF->size = real_MAXSIZE;
    }
}   

Cache::~Cache(){
	delete rp;
	delete s_engine;
}

Data* Cache::read(int addr) {  
    sr = newBST->searchNode(root, addr);
    if (sr == NULL)
        return NULL;
    else if(sr != NULL && flag == false){ 
        Node* rep = sr;
        rep->Freq++;
        if(rep->flag_RW == false)
            rep->newN->sync = false;
        else 
            rep->newN->sync = true;

        if(this->rp->flagML == 0){
            M->newH->deleteHeap(M->newH->arr, sr);
            M->newH->insertHeap(rep);
        }
        else if(this->rp->flagML == 1){
            L->newH->deleteHeap(L->newH->arr, sr);
            L->newH->insertHeap(rep);
        }
        else if(this->rp->flagML == 2)
            MF->incrementFeq(sr);
        else 
            LF->incrementFeq(sr);
        return rep->newN->data;    
    }
    else 
        return sr->newN->data;
}

Elem* Cache::put(int addr, Data* cont) {
    Elem* newE = new Elem(addr, cont, true);
    Node* newN = new Node(newE);
    newN->flag_RW = true;
    
    if(newBST->p < MAXSIZE){
        newBST->insertBST(root, newN);
        if(this->rp->flagML == 0){
            M->newH->insertHeap(newN);
            M->newH->insertPrime(newN, 0);
        }
        else if(this->rp->flagML == 1){
            L->newH->insertHeap(newN);
            L->newH->insertPrime(newN, 0);
        }
        else if(this->rp->flagML == 2){
            MF->insertMFRU(newN);
            MF->insertPrime(newN, 0);
        }
        else{
            LF->insertLFRU(newN);
            LF->insertPrime(newN, 0);
        }
        return NULL;
    }   
    else {
        if(this->rp->flagML == 0){
            Node* ret = &M->newH->arr[0];
            int idx = M->newH->get_idx(ret);
            root = newBST->deleteBST(root, ret->newN->addr);
            M->newH->deleteHeap(M->newH->arr, &M->newH->arr[0]);
            newBST->insertBST(root, newN);
            M->newH->insertHeap(newN);
            M->newH->insertPrime(newN, idx);
            return ret->newN;
        }
        else if(this->rp->flagML == 1){
            Node* ret = &L->newH->arr[0];
            int idx = L->newH->get_idx(ret);
            root = newBST->deleteBST(root, ret->newN->addr);
            L->newH->deleteHeap(L->newH->arr, &L->newH->arr[0]);
            newBST->insertBST(root, newN);
            L->newH->insertHeap(newN);
            L->newH->insertPrime(newN, idx);
            return ret->newN;
        }
        else if(this->rp->flagML == 2){
            int idx = MF->get_idx(&MF->arr[0]);
            root = newBST->deleteBST(root, MF->arr[0].newN->addr);
            MF->deleteMFRU();
            newBST->insertBST(root, newN);
            MF->insertMFRU(newN);
            MF->insertPrime(newN, idx);
            return MF->arr[0].newN;
        }
        else {
            int idx = LF->get_idx(&LF->arr[0]);
            root = newBST->deleteBST(root, LF->arr[0].newN->addr);
            LF->deleteLFRU();
            newBST->insertBST(root, newN);
            LF->insertLFRU(newN);
            LF->insertPrime(newN, idx);
            return LF->arr[LF->curr].newN;
        }
    }
}

Elem* Cache::write(int addr, Data* cont) {
    Elem* newE = new Elem(addr, cont, false);
    Node* newN = new Node(newE);
    flag = true;
    newN->flag_RW = false;

    if(read(addr) == NULL && newBST->p < MAXSIZE){
        newBST->insertBST(root, newN);
        if(this->rp->flagML == 0){
            M->newH->insertHeap(newN);
            M->newH->insertPrime(newN, 0);
        }
        else if(this->rp->flagML == 1){
            L->newH->insertHeap(newN);
            L->newH->insertPrime(newN, 0);
        }
        else if(this->rp->flagML == 2){
            MF->insertMFRU(newN);
            MF->insertPrime(newN, 0);
        }
        else{
            LF->insertLFRU(newN);
            LF->insertPrime(newN, 0);
        }
        flag = false;
        return NULL;
    }
    
    else if (read(addr) != NULL && flag == true) {
        Node* cr = sr;
        Node* sp = cr;
        cr->Freq++;
        cr->newN->data = cont;
        cr->newN->sync = false;
        cr->flag_RW = false;
        if(this->rp->flagML == 0){
            M->newH->deleteHeap(M->newH->arr, sp);
            M->newH->insertHeap(cr);
        }
        else if(this->rp->flagML == 1){
            L->newH->deleteHeap(L->newH->arr, sp);
            L->newH->insertHeap(cr);
        }
        else if(this->rp->flagML == 2){
            MF->incrementFeq(cr);
        }
        else 
            LF->incrementFeq(cr);
        flag = false;
        return NULL;
    }

    else {         
        if(this->rp->flagML == 0) {
            int index = M->newH->get_idx(&M->newH->arr[0]);
            root = newBST->deleteBST(root, M->newH->arr[0].newN->addr);
            M->newH->deleteHeap(M->newH->arr, &M->newH->arr[0]);
            newBST->insertBST(root, newN);
            M->newH->insertHeap(newN);
            M->newH->insertPrime(newN, index);    
        }
        else if(this->rp->flagML == 1) {
            int index = L->newH->get_idx(&L->newH->arr[0]);
            root = newBST->deleteBST(root, L->newH->arr[0].newN->addr);
            L->newH->deleteHeap(L->newH->arr, &L->newH->arr[0]);
            newBST->insertBST(root, newN);
            L->newH->insertHeap(newN);
            L->newH->insertPrime(newN, index);
        }
        else if(this->rp->flagML == 2) {
            int index = MF->get_idx(&MF->arr[0]);
            root = newBST->deleteBST(root, MF->arr[0].newN->addr);
            MF->deleteMFRU();
            newBST->insertBST(root, newN);
            MF->insertMFRU(newN);
            MF->insertPrime(newN, index);   
        }
        else {
            int index = LF->get_idx(&LF->arr[0]);
            root = newBST->deleteBST(root, LF->arr[0].newN->addr);
            LF->deleteLFRU();
            newBST->insertBST(root, newN);
            LF->insertLFRU(newN);
            LF->insertPrime(newN, index);
        }
        flag = false;
    }
}

void Cache::printRP() {
    if(root != NULL){
        if(this->rp->flagP == 0) 
            M->printMFU();
        else if(this->rp->flagP == 1)
            L->printLFU();
        else if(this->rp->flagP == 2)
            MF->printMFRU();
        else
            LF->printLFRU(); 
    }
}

void Cache::printLP() {
    cout<<"Prime memory\n";
    if(root != NULL){
        if(this->rp->flagML == 0) {
            for(int i = 0; i <= M->newH->lpos; i++)
                M->newH->PrimeArray[i].newN->print();
            cout<<"Hash table memory\n";
            M->newH->Hashtable();
        }
        else if(this->rp->flagML == 1) {
            for(int i = 0; i < L->newH->size; i++)
                L->newH->PrimeArray[i].newN->print();
            cout<<"Hash table memory\n";
            L->newH->Hashtable();
        }  
        else if(this->rp->flagML == 2) {
            MF->lpos = MF->curr;
            for(int i = 0; i < MF->size; i++){
                MF->PrimeArray[i].newN->print();
            }
            cout<<"Hash table memory\n";
            MF->Hashtable();
        }
        else {
            LF->lpos = LF->curr;
            for(int i = 0; i < LF->size; i++)
                LF->PrimeArray[i].newN->print();
            cout<<"Hash table memory\n";
            LF->Hashtable();
        }
    }
}

void Cache::printSE() {
    cout<<"Print BST in inorder:\n";
    newBST->printInorder(root);
    cout<<"Print BST in preorder:\n";
    newBST->printPreorder(root);
}