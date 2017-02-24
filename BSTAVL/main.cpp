#include "BSTAVL.h"

int main() {

    BSTAVL<int> * bst = new BSTAVL<int>();

    /*bst->insert(41);
    bst->insert(22);
    bst->insert(58);
    bst->insert(19);
    bst->insert(33);
    bst->insert(50);
    bst->insert(63);
    bst->insert(13);
    bst->insert(30);
    bst->insert(37);
    bst->insert(42);
    bst->insert(53);
    bst->insert(14);
    bst->insert(16);
    bst->insert(17);
    bst->insert(18);*/

    /*bst->insert(5);
    bst->insert(3);
    bst->insert(6);
    bst->insert(2);
    bst->insert(4);
    bst->insert(1);
    bst->insert(7);
    bst->insert(10);
    bst->insert(12);*/

    bst->insert(1);
    bst->insert(2);
    bst->insert(3);
    bst->insert(4);
    bst->insert(5);
    bst->insert(6);
    bst->insert(7);
    bst->insert(8);
    bst->insert(9);
    bst->insert(10);



    bst->leverOrder();

    cout << endl;

    //bst->delNode(1);

    //bst->leverOrder();
    delete bst;

    return 0;
}