//
// Created by zhangsiqi on 2017/2/21.
//

#ifndef BSTAVL_BSTAVL_H
#define BSTAVL_BSTAVL_H

#include <iostream>
#include <cassert>
#include <queue>

using namespace std;

template <typename Key>
class BSTAVL {

private:
    struct Node{
        Key key;
        int depth;
        int balance;
        Node* parent;
        Node* left;
        Node* right;

        Node(Key data){
            this->key = data;
            depth = 1;
            balance = 0;
            left = NULL;
            right = NULL;
            parent = NULL;
        }

        Node(Node* node){
            key = node->key;
            depth = 1;
            balance = 0;
            parent = NULL;
            left = NULL;
            right = NULL;
        }
    };
    Node* root;
    int count;

public:

    BSTAVL(){
        root = NULL;
        count = 0;
    }

    void insert(Key data){
        if( root == NULL ){
            root = new Node(data);
            count++;
            return;
        }
        insert(root, data);
    }

    bool search(Key key){
        return contain(key);
    }

    // 广度优先遍历 (层序遍历)
    void leverOrder(){

        queue<Node*> q;
        q.push(root);   // 首先存入根节点
        while( !q.empty() ) {   // 当队列不为空

            Node *node = q.front();  // 取出队列头
            q.pop();                 // 弹出队列头
            cout << node->key << " ";
            if(node->left)           // 压入该结点的左孩子
                q.push(node->left);
            if(node->right)          // 压入该结点的右孩子
                q.push(node->right);
        }
        cout << endl;
    }
    // 中序遍历
    void inorder(){
        inorder(root);
        cout << endl;
    }

    void delNode(Key key){
        assert( contain(key) );
        delNode(root, key);
    }

private:

    void insert( Node* node, Key data ){
        if( data < node->key ){
            if( node->left != NULL )
                insert(node->left, data);
            else{ // 左节点为空
                Node* node1 = new Node(data);
                node1->parent = node;
                node->left = node1;
                count++;
            }
        }
        else if( data > node->key ){
            if( node->right != NULL )
                insert( node->right , data );
            else{
                Node* node1 = new Node(data);
                node1->parent = node;
                node->right = node1;
                count++;
            }
        }
        // 将插入后的二叉树进行平衡调整
        Balance(node);
    }

    bool contain(Key key){

        Node* node1 = root;
        while(1){
            while( key < node1->key && node1 != NULL ){
                node1 = node1->left;
                if( node1 == NULL )
                    return false;
            }

            while( key > node1->key ){
                node1 = node1->right;
                if( node1 == NULL )
                    return false;
            }
            if( node1->key == key )
                return true;
        }
    }

    Node* delNode(Node* node, Key key){
        if( node == NULL )
            return NULL;

        if( key > node->key ) {
            node->right = delNode(node->right, key);
            Balance(node);
            return node;
        }
        else if( key < node->key ){
            node->left = delNode(node->left, key);
            Balance(node);
            return node;
        }
        else{ // key == node->key
            if( node->left == NULL ){  // 左节点为空保存右节点
                Node* rightNode = node->right; // 保存右节点
                if( rightNode != NULL ){
                    rightNode->parent = node->parent; // 保存父节点
                    Balance(rightNode->parent);
                }

                delete node;
                count--;
                return rightNode;  // 返回右节点给父节点
            }
            if( node->right == NULL ){
                Node* leftNode = node->left; // 保存左节点
                if( leftNode != NULL ){
                    leftNode->parent = node->parent;     // 保存父节点
                    Balance(leftNode->parent);
                }

                delete node;
                count--;
                return leftNode;  // 返回左节点给父节点
            }
            // node->left != NULL && node->right != NULL
            Node* successor = new Node(mimimum(node->right));  // 查找右节点的最小值替换删除节点
            successor->right = removeMin(node->right);  // 新替换节点指向删除最小值后的右子树
            successor->left = node->left;
            successor->parent = node->parent;
            delete node;
            Balance(successor->parent);
            return successor;
        }
    }

    Node* mimimum(Node* node){
        Node* node1 = node;
        while( node1->left != NULL )
            node1 = node->left;
        return node;
    }

    Node* removeMin(Node* node){
        if( node == NULL )
            return NULL;
        if( node->left != NULL ) {
            node->left = removeMin(node->left);
            return node;
        }
        delete node;
        count--;
        return NULL;
    }

    void inorder(Node* node){
        if( node == NULL )
            return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    // 右旋
    void right_rotate( Node* node ){
        // 一次旋转涉及到的节点包括 双亲,左子做父,右孙
        Node* pParent = node->parent, *pLeftSon = node->left, *pLeftGrandSon = pLeftSon->right;

        // 左子做父
        pLeftSon->parent = pParent;
        if( pParent != NULL ){  // 存在父节点
            if( node == pParent->left )  // node 为左子
                pParent->left = pLeftSon;  // 将 node -> parent -> left 指向 node -> left
            else if( node == pParent->right ) // node 为右子
                pParent->right = pLeftSon;
        }
        else
            root = pLeftSon;
        // 根为右子
        pLeftSon->right = node;
        node->parent = pLeftSon;

        // 右孙 变 左孙
        node->left = pLeftGrandSon;
        if( pLeftGrandSon != NULL )
            pLeftGrandSon->parent = node;

        // 重新计算平衡因子
        node->depth = calcDepth(node);
        node->balance = calcBalance(node);

        pLeftSon->depth = calcDepth(pLeftSon);
        pLeftSon->balance = calcBalance(pLeftSon);
    }

    // 左旋
    void left_rotate( Node* node ){
        // 一次旋转设计到的节点包括 双亲, 右子做父, 左孙
        Node* pParent = node->parent, *pRightSon = node->right,*pRightGrandSon = pRightSon->left;

        // 右子做父
        pRightSon->parent = pParent;
        if( pParent != NULL ){
            if( node == pParent->left )
                pParent->left = pRightSon;
            else if( node == pParent->right )
                pParent->right = pRightSon;
        }
        else
            root = pRightSon;
        // 根为左子
        pRightSon->left = node;
        node->parent = pRightSon;

        // 左孙 变 右孙
        node->right = pRightGrandSon;
        if( pRightGrandSon != NULL )
            pRightGrandSon->parent = node;

        // 重新计算平衡因子
        node->depth = calcDepth(node);
        node->balance = calcBalance(node);

        pRightSon->depth = calcDepth(pRightSon);
        pRightSon->balance = calcBalance(pRightSon);
    }

    // 高度差
    int calcBalance(Node* node){  // 以传入的node节点为根 计算左右两字数的高度差
        int left_depth;
        int right_depth;

        if( node->left != NULL )
            left_depth = node->left->depth;
        else left_depth = 0;
        if( node->right != NULL )
            right_depth = node->right->depth;
        else right_depth = 0;

        return left_depth - right_depth;
    }

    // 深度
    int calcDepth( Node* node ){ // 计算当前节点为根节点 高度高的子树深度
        int depth = 0;
        if( node->left != NULL )
            depth = node->left->depth;

        if( node->right != NULL && depth < node->right->depth )
            depth = node->right->depth;

        depth++;
        return depth;
    }

    void Balance(Node*node){
        node->balance = calcBalance(node);

        // 左子树高,应该右旋
        if( node->balance >= 2 ){
            // 如果左子树右孙高, 先左旋
            if( node->left->balance == -1 )
                left_rotate(node->left);
            // 右旋
            right_rotate(node);
        }

        if( node->balance <= -2 ){
            // 如果右子树左孙高,先右旋
            if(node->right->balance == 1)
                right_rotate( node->right );
            // 左旋
            left_rotate(node);
        }

        node->balance = calcBalance(node); // 重新计算平衡因子
        node->depth = calcDepth(node);     // 重新计算当前节点深度
    }

};


#endif //BSTAVL_BSTAVL_H
