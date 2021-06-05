
/**
 * Author: Mayan Bashan
 */
#pragma once
#include <map>
#include <queue>
#include <string>
#include <iterator>
#include <iostream>
#include <stack>
using namespace std;

namespace ariel{
    
    template<typename T>
    class BinaryTree{
        private:
            struct Node{
                    T data;
                    Node* left;
                    Node* right;
                    Node(T data) : data(data){
                        this->left = nullptr;
                        this->right = nullptr;
                    }
                    // copy constructor
                    Node(const Node &other) : data(other.data) , left(other.left), right(other.right){
                        if(other){
                            Node* node = new Node();
                            node->data = other->data;
                            node->left = other->left;
                            node->right = other->right;
                            return node;
                        }
                        return new Node(nullptr);
                    }
            
                    // move constructor
                    Node(Node&& other) noexcept{
                        data = other.data;
                        left = other.left;
                        right = other.right;
                        other.data = nullptr;
                        other.left = nullptr;
                        other.right = nullptr;
                    }

                    // move assignment operator
                    Node& operator=(Node&& other) noexcept{ //noexcept
                        data = other.data;
                        left = other.left;
                        right = other.right;
                        other.data = nullptr;
                        other.left = nullptr;
                        other.right = nullptr;
                        return *this;
                    }

                    // copy assignment
                    Node& operator=(Node other){
                        data = other.data;
                        left = new Node(other.left->data);
                        right = new Node(other.right->data);
                        return *this;
                    }

                    //Node();
                    ~Node(){
                        delete right;
                        delete left;
                    }
            };
            Node* root = nullptr;

        public:
            // constructors
            BinaryTree(){};
            ~BinaryTree(){}
            BinaryTree(const Node& _root) : root(_root){};

            // copy constructor
            BinaryTree(const BinaryTree &other) : root(nullptr){
                std::vector<Node*> remaining;
                Node* cur = other.root;
                remaining.push_back(cur);
                this->add_root(cur->data);
                while(!remaining.empty()){
                    Node* curr = remaining.back();
                    remaining.pop_back();
                    if(curr->left){
                        this->add_left(curr->data, curr->left->data);
                        remaining.push_back(curr->left);
                    }
                    if(curr->right){
                        this->add_right(curr->data, curr->right->data);
                        remaining.push_back(curr->right);
                    }
                }
            }

            // copy assignment
            BinaryTree& operator=(BinaryTree other){
                root = other.root;
                return *this;
            }
            
            // move constructor
            BinaryTree(BinaryTree&& other) noexcept{
                root = other.root;
                other.root = nullptr;
            }

            // move assignment operator
            BinaryTree& operator=(BinaryTree&& other) noexcept{
                if(root){
                    delete root;
                }
                root = other.root;
                other.root = nullptr;
                return *this;
            }

           

            BinaryTree& add_root(T element) {
                if (root == nullptr){
                    this->root = new Node(element);
                }
                else{
                    this->root->data = element;
                }
                return *this;
            }
           
            BinaryTree& add_left(T exist, T element){
                Node* curr = nullptr;
                std::queue<Node*> q;
                if (this->root == nullptr){
                    throw("Tree does not contain element");
                }
                int flag = 0;
                q.push(this->root);
                while(!q.empty()){
                    curr = q.front();
                    q.pop();
                    if (curr->data == exist){
                        flag = 1;
                        break;
                    }
                    if(curr->left != nullptr){
                        q.push(curr->left);
                    }                
                    if(curr->right != nullptr){
                        q.push(curr->right);
                    }
                }
                if (flag == 1){
                    if (curr->left == nullptr){
                        curr->left = new Node(element);
                    }
                    else{
                        curr->left->data = element;
                    }
                    return *this;
                }
                throw("Tree does not contain element");
            }

            BinaryTree& add_right(T exist, T element){
                Node* curr = nullptr;
                std::queue<Node*> q;
                if (this->root == nullptr){
                    throw("Tree does not contain element");
                }
                int flag = 0;
                q.push(this->root);
                while(!q.empty()){
                    curr = q.front();
                    q.pop();
                    if (curr->data == exist){
                        flag = 1;
                        break;
                    }
                    if(curr->left != nullptr){
                        q.push(curr->left);
                    }                
                    if(curr->right != nullptr){
                        q.push(curr->right);
                    }
                }
                if (flag == 1){
                    if (curr->right == nullptr){
                        curr->right = new Node(element);
                    }
                    else{
                        curr->right->data = element;
                    }
                    return *this;
                }
                 throw("Tree does not contain element");
            }

            friend ostream& operator <<(ostream& output, const BinaryTree& tree){
                return output;
            }
        
            class iterator{
                public:
                    Node* n;
                    std::queue<Node*> myqueue;

                    iterator(Node* ptr = nullptr) : n(ptr) , myqueue() {}

                    bool operator==(const iterator& other) const{
                        return n == other.n;
                    }
                    bool operator!=(const iterator& other) const{
                        return n!=other.n;
                    }
                    T* operator->() const{
                        return &(n->data);
                        //return new T();
                    }
                    T& operator*() const{
                        return n->data;
                    }
        
                    iterator operator ++(){ //++it
                        if(this->n == nullptr){
                            return *this;
                        }
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return *this;
                            Node* node = nullptr;
                            return iterator(node);
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return *this;
                    }

                    iterator operator++(int){ //it++
                        if(this->n == nullptr){
                            return *this;
                        }
                        iterator it_copy(*this);
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return it_copy;
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return it_copy;
                    } 
            };
            
            // =============== Preorder ===============
            class IteratorPre : public iterator{
                public:
                    //IteratorPre(){}
                    IteratorPre(Node* ptr) : iterator(ptr){
                        if(ptr == nullptr){
                            this->n = nullptr;
                        }
                        else{
                            init_queue(ptr);
                            this->n = this->myqueue.front();
                            this->myqueue.pop(); 
                        }
                    }
                    
                    void init_queue(Node* root){
                        if(root==nullptr){
                            return;
                        }
                        this->myqueue.push(root);
                        init_queue(root->left);
                        init_queue(root->right);
                    }

                    IteratorPre operator ++(){ //++it
                       if(this->n == nullptr){
                            return *this;
                        }
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return *this;
                            Node* node = nullptr;
                            return IteratorPre(node);
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return *this;
                    }

                    IteratorPre operator++(int){ //it++
                        if(this->n == nullptr){
                            return *this;
                        }
                        IteratorPre it_copy(*this);
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return it_copy;
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return it_copy;
                    }
            };

            // =============== Inorder ===============
            class IteratorIn : public iterator{
                public:
                    IteratorIn(Node* ptr) : iterator(ptr){
                        if(ptr == nullptr){
                            this->n = nullptr;
                        }
                        else{
                            init_queue(ptr);
                            this->n = this->myqueue.front();
                            this->myqueue.pop(); 
                        }
                    }
                    
                    void init_queue(Node* root){
                        if(root==nullptr){
                            return;
                        }
                        init_queue(root->left);
                        this->myqueue.push(root);
                        init_queue(root->right);
                    }
                    
                    IteratorIn operator ++(){ //++it
                       if(this->n == nullptr){
                            return *this;
                        }
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return *this;
                            Node* node = nullptr;
                            return IteratorIn(node);
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return *this;
                    }

                    IteratorIn operator++(int){ //it++
                        if(this->n == nullptr){
                            return *this;
                        }
                        IteratorIn it_copy(*this);
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return it_copy;
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return it_copy;
                    } 
            };

            // =============== Postorder ===============
            class IteratorPost : public iterator{
                public:
                    IteratorPost(Node* ptr) : iterator(ptr){
                        if(ptr == nullptr){
                            this->n = nullptr;
                        }
                        else{
                            init_queue(ptr);
                            this->n = this->myqueue.front();
                            this->myqueue.pop(); 
                        }
                    }
                    
                    void init_queue(Node* root){
                        if(root==nullptr){
                            return;
                        }
                        init_queue(root->left);
                        init_queue(root->right);
                        this->myqueue.push(root);
                    }

                    IteratorPost operator ++(){ //++it
                        if(this->n == nullptr){
                            return *this;
                        }
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return *this;
                            Node* node = nullptr;
                            return IteratorPost(node);
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return *this;
                    }

                    IteratorPost operator++(int){ //it++
                        if(this->n == nullptr){
                            return *this;
                        }
                        IteratorPost it_copy(*this);
                        if(this->myqueue.size() == 0){
                            this->n = nullptr;
                            return it_copy;
                        }
                        this->n = this->myqueue.front();
                        this->myqueue.pop();
                        return it_copy;
                    } 
                    
            };

            iterator begin(){
                return begin_inorder();
            }
            iterator end(){
                return end_inorder();
            }
            
            IteratorPre begin_preorder(){
                return IteratorPre(root);
            }
            IteratorPre end_preorder(){
                Node* node = nullptr;
                return IteratorPre(node);
            }

            IteratorIn begin_inorder(){
                return IteratorIn(root);
            }
            IteratorIn end_inorder(){
                Node* node = nullptr;
                return IteratorIn(node);
            }

            IteratorPost begin_postorder(){ 
                return IteratorPost(root);
            }
            IteratorPost end_postorder(){
                Node* node = nullptr;
                return IteratorPost(node);
            }
    };
}