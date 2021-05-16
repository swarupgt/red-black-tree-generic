#include "Node.h"
#include<iostream>
using namespace std;

#define DISPLAY_2D_SPACING_TABS 2


template <typename T>
struct lessThan
{
    bool operator()( const T& lhs, const T& rhs )
    {
        return lhs < rhs;
    }
};


template< typename T, typename binop_t = lessThan<T> >
class RBTree
{            
    private:
        Node<T>* root;

        binop_t op;
        
        //implementation functions
        Node<T>* BSTinsert_r(Node<T>* iter, Node<T>* n);
        Node<T>* correctTreeInsert(Node<T>* iter);          //fixes consecutive red nodes after insertion
        void correctTreeDelete(Node<T>* iter);          //fixes double black nodes after deletion
        void delete_r(Node<T>* iter);
        Node<T>* BSTreplacement(Node<T>& );
        Node<T>* remove_r(Node<T>*);
        Node<T>* search_r(Node<T>* iter, const T&);
        void leftRotate( Node<T>* );
        void rightRotate( Node<T>* );
        void swapColors(Node<T>*, Node<T>*);
        void swapValues(Node<T>*, Node<T>*);
        void disp_r(Node<T>* iter);
        void disp2d_r( Node<T>*, int );

    public:
        RBTree();
        explicit RBTree(T obj);
        ~RBTree();
        

        class Iterator
        {
            

            private:
                Node<T>* p_it_;


            public:
                
                //iterator traits
                using iterator_category = std::bidirectional_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = T;
                using pointer           = T*;  // or Node<T>* ?
                using reference         = T&;
                
                Iterator() = delete;

                explicit Iterator(Node<T>* p_it);
                Iterator& operator++();
                Iterator operator++(int);
                Iterator& operator--();
                Iterator operator--(int);
                const T& operator*();
                bool operator==(const Iterator&) const; 
                bool operator!=(const Iterator&) const;

        };

        //interface functions
        Iterator begin();
        Iterator end();

        void disp();

        void disp2d();

        Iterator search(const T&);

        Iterator insert( const T& );

        //3 overloads - value_type, Iterator and Iterator range
        Iterator erase( const T& );
        

         // these 2 overloads are left
        Iterator erase( Iterator, Iterator ); 
        Iterator erase(Iterator);



};


template<typename T, typename binop_t>
RBTree<T, binop_t>::RBTree() : root(nullptr)
{
    // cout << "def ctor called\n";
}

template<typename T, typename binop_t>
RBTree<T, binop_t>::RBTree(T obj): root(new Node<T>(obj))
{
    // cout << "non-def ctor called\n";
    root->setColor(BLACK);
}

template<typename T, typename binop_t>
void RBTree<T, binop_t>::delete_r(Node<T>* iter)
{
    if(iter != nullptr)
    {
        delete_r(iter->left);
        delete_r(iter->right);

        delete iter;
    }
    
}


template<typename T, typename binop_t>
RBTree<T, binop_t>::~RBTree()
{
    if(root != nullptr)
        delete_r(root);

    cout << "Red Black Tree deleted.\n";
}


//-----------------------ITERATORS DEFINITIONs------------------------------------

#if 1

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::begin()
{
    if(root == nullptr)
        return Iterator(nullptr);
    
    Node<T>* temp = root;
    // cout << "finding begin(), currently at root : " << root->getObj() << "\n";
    while(temp->left != nullptr){
        temp = temp->left;
        // cout << "this is to the left : " << temp->getObj() << "\n";
    }
    
    // cout << "returning this : " << temp->getObj() << "\n";
    return Iterator(temp);
}

// end is complete
template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::end()
{
    if(!root)
        return Iterator(nullptr);
    
    Node<T>* temp = root;
    while( temp->right != nullptr )
        temp = temp->right;
    
    return Iterator(temp+1);
}

template<typename T, typename binop_t>
RBTree<T, binop_t>::Iterator::Iterator(Node<T> *p_it) : p_it_(p_it)
{ }

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator& RBTree<T, binop_t>::Iterator::operator++()
{
    this->p_it_ = this->p_it_->inOrderSuccessor();
    return *this;
}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::Iterator::operator++(int)
{
    Iterator temp(*this);

    this->p_it_ = this->p_it_->inOrderSuccessor();

    return temp;
}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator& RBTree<T, binop_t>::Iterator::operator--()
{
    this->p_it_ = this->p_it_->inOrderPredecessor();
    return *this;
}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::Iterator::operator--(int)
{
    Iterator temp(*this);

    this->p_it_ = this->p_it_->inOrderPredecessor();

    return temp;
}

template<typename T, typename binop_t>
bool RBTree<T, binop_t>::Iterator::operator==( const RBTree<T, binop_t>::Iterator &rhs) const
{
    return p_it_ == rhs.p_it_;
}

template<typename T, typename binop_t>
bool RBTree<T, binop_t>::Iterator::operator!=( const RBTree<T, binop_t>::Iterator &rhs) const
{
    return !( *this == rhs );
}

template<typename T, typename binop_t>
const T& RBTree<T, binop_t>::Iterator::operator*()
{
    return p_it_->getObj();
}


//----------------------------------RBTREE DEFINITIONS--------------------------------------



template<typename T, typename binop_t>
void RBTree<T, binop_t>::disp_r(Node<T>* iter)       //displays in-order
{
    if(iter == nullptr)
        return;
    
    disp_r(iter->left);
    if( iter->getColor() == RED ) cout << "RED\t";
    else cout << "BLACK\t";

    if( root == iter )
    {
        cout << iter->getObj() <<"\tis the root\n";
    }
    else
    {
        cout << iter->getObj() << "\tis a left child: " << iter->isOnLeft() << "\t& parent is: " << iter->parent->getObj() <<"\n";
    }

    disp_r(iter->right);

}


template<typename T, typename binop_t>
void RBTree<T, binop_t>::disp()
{   
    if(root == nullptr)
        cout << "Tree is empty.\n";
    
    else
        disp_r(root);
    // cout << root->getObj() << " is the root\n";
    cout << '\n';
}

template<typename T, typename binop_t>
void RBTree<T, binop_t>::disp2d_r( Node<T>* iter, int space )       // 2d display
{
    if(iter == nullptr)
        return;

    space += DISPLAY_2D_SPACING_TABS;
    
    disp2d_r( iter->right, space );

    cout << endl;

    

    for( int i = DISPLAY_2D_SPACING_TABS; i < space; ++i )
        cout << "\t";

    if( iter->getColor() == RED ) cout << "RED   ";
    else cout << "BLACK ";

    cout << iter->getObj() << endl;

    // if( root == iter )
    // {
    //      <<"\tis the root\n";
    // }
    // else
    // {
    //     cout << iter->getObj() << "\tis a left child: " << iter->isOnLeft() << "\t& parent is: " << iter->parent->getObj() <<"\n";
    // }

    disp2d_r(iter->left, space);

}

template<typename T, typename binop_t>
void RBTree<T, binop_t>::disp2d()                       // 2d display
{   
    if(root == nullptr)
        cout << "Tree is empty.\n";
    
    else
        disp2d_r( root, 0 );
    // cout << root->getObj() << " is the root\n";
    cout << '\n';
}


template<typename T, typename binop_t>
void RBTree<T, binop_t>::swapColors( Node<T>* x, Node<T>* y )
{
    COLOR colorx = x->getColor();
    x->setColor( y->getColor() );
    y->setColor( colorx );
}

template<typename T, typename binop_t>
void RBTree<T, binop_t>::swapValues(Node<T>* x, Node<T>* y)
{
    T temp = x->getObj();
    x->setObj(y->getObj());
    y->setObj( temp );
}


template<typename T, typename binop_t>
void RBTree<T, binop_t>::leftRotate( Node<T>* toBeRot )
{

    Node<T> *newParent = toBeRot->right;
 
    if ( toBeRot == root )
        root = newParent;
 
    toBeRot->moveDown( newParent );
 
    toBeRot->right = newParent->left;

    if (newParent->left != nullptr)
      newParent->left->parent = toBeRot;
 
    newParent->left = toBeRot;

}


template<typename T, typename binop_t>
void RBTree<T, binop_t>::rightRotate( Node<T> *toBeRot )
{

    Node<T> *newParent = toBeRot->left;
 
    if (toBeRot == root)
      root = newParent;
 
    toBeRot->moveDown(newParent);
 
    toBeRot->left = newParent->right;
    
    if (newParent->right != nullptr)
      newParent->right->parent = toBeRot;
 
    newParent->right = toBeRot;
    
}

template<typename T, typename binop_t>
Node<T>* RBTree<T, binop_t>::correctTreeInsert( Node<T>* iter )
{

    if( root == iter )
    {
        // cout << "first if of ct\niter: ";
        iter->setColor(BLACK);
        //iter->dispNode();
        return iter;
    }

    Node<T>* Parent = iter->parent, *Grandparent = iter->parent->parent, *Uncle = iter->retUncle();    

    // cout << "else of first if of ct\niter: ";
    //iter->dispNode();
    if( Parent->getColor() != RED ){
        // cout << "parent is black and child is red\n";
        return iter;
    }
    else if( Uncle != nullptr && Uncle->getColor() == RED )       //if uncle exists and is red
    //else if( Uncle->getColor() == RED )
    {
        // cout << "p & c are red, uncle also red\n";
        //set parent and uncle to black
        Parent->setColor(BLACK);
        Uncle->setColor(BLACK);
        //iter is now it's grandparent
        Grandparent->setColor(RED);

        return correctTreeInsert(Grandparent);        //set iter to grandparent

    }

    else if( iter->isOnLeft() )
    {
        // cout << "iter is a left ch\n";
        if( Grandparent != nullptr && Parent->isOnLeft() )
        //if( Parent->isOnLeft() )
        {
            //ll case
            rightRotate(Grandparent);
            swapColors(Parent, Grandparent);      //swap gp and p colors 

        }

            //RL case
        else if( Parent != nullptr && Grandparent != nullptr )
        //else 
        {

            rightRotate( Parent);
        
            leftRotate( Grandparent );

            // cout << "\n\n\nInside RL case, iter: " << iter->getObj() << "\n\ngrandpar: " << Grandparent->getObj() << '\n';
            swapColors( iter, Grandparent );
        }

            
    }
        

    else if(Parent!= nullptr && Grandparent != nullptr && Parent->isOnLeft() )
    {
        //LR case

        
        leftRotate( Parent );
        rightRotate( Grandparent );
        swapColors( iter, Grandparent );

        // cout << "grandparent:" << Grandparent->getObj() << "\n";
        
    }
            
    //rr case
    else if( Parent != nullptr && Grandparent != nullptr)
    {
        leftRotate( Grandparent );
        swapColors( Parent, Grandparent );
        
    }
        
    return iter;
     
}

template<typename T, typename binop_t>
void RBTree<T, binop_t>::correctTreeDelete(Node<T>* iter)
{
    if( iter == root )
        return;
    
    Node<T> *sib = iter->retSibling();
    Node<T> *par = iter->parent;

    if( sib == nullptr )
        correctTreeDelete( par );
    else
    {
        if( sib->getColor() == RED )
        {
            par->setColor( RED );
            sib->setColor( BLACK );

            if( sib->isOnLeft() )
                rightRotate( par );
            else
                leftRotate( par );
            correctTreeDelete( iter );
        }

        else
        {
            if( sib->hasRedChild() )
            {
                if( sib->left != nullptr && sib->left->getColor() == RED )
                {
                    if( sib->isOnLeft() )
                    {
                        // left left
                        sib->left->setColor( sib->getColor() );
                        sib->setColor( par->getColor() );
                        rightRotate( par );
                    }

                    else
                    {
                        // right left
                        sib->left->setColor( par->getColor() );
                        rightRotate( sib );
                        leftRotate( par );
                    }
                }

                else
                {
                    if( sib->isOnLeft() )
                    {
                        // left right
                        sib->right->setColor( par->getColor() );
                        leftRotate( sib );
                        rightRotate( par );
                    }
                    else
                    {
                        // right right
                        sib->right->setColor( sib->getColor() );
                        sib->setColor( par->getColor() );
                        leftRotate( par );
                    }
                }
                
                par->setColor( BLACK );
            }

            else
            {
                sib->setColor( RED );
                if( par->getColor() == BLACK )
                    correctTreeDelete( par );
                else
                    par->setColor( BLACK );
            }
        }
    }
} 


template< typename T, typename binop_t >
Node<T>* RBTree<T, binop_t>::BSTinsert_r(Node<T>* iter, Node<T>* newNode)
{
    if( iter == nullptr )
    {
        // cout << "BSTinsert_r() is returning " << newNode->getObj() <<'\n';
        return newNode;
    }

    // cout << "inside BSTinsert_r()\n";
    newNode->parent = iter;

    if( op( newNode->getObj(), iter->getObj() ) )
    {
        // cout << "obj is less than whatever\n";
        // cout << iter->getObj() << " going left \n";
        iter->left = BSTinsert_r( iter->left, newNode );
        
    }   
    else
    {
        // cout << "obj is greater than whatever\n";
        // cout << iter->getObj() << " going right \n";
        iter->right = BSTinsert_r( iter->right, newNode );
        
    }

    // cout << "BSTinsert_r() is returning " << iter->getObj() <<'\n';
    return iter;
}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::insert( const T& obj )
{
    
    Node<T>* newNode = new Node<T>(obj);

    if(root == nullptr)
    {
        //set black if root
        root = newNode;
        newNode->setColor(BLACK);
        return Iterator(root);
    }

    BSTinsert_r( root, newNode);
    
    // cout << "newNode: " << newNode->getObj() << "\nparent of newNode: " << newNode->parent->getObj() << "\n";
    
    newNode = correctTreeInsert( newNode );

    // cout << "final root: " << root->getObj() <<"\n";

    return Iterator( newNode );
}


template<typename T, typename binop_t>
Node<T>* RBTree<T, binop_t>::BSTreplacement( Node<T>& res )
{
    if(res.left == nullptr && res.right == nullptr)
        return nullptr;
    
    //case 2 - node only has one child
    else if(res.left && !res.right || !res.left && res.right)
    {
        if(res.left)
            return res.left;

        if(res.right)
            return res.right;
    }

    //case 3
    else if(res.right && res.left)
        return res.inOrderSuccessor();   
}


template<typename T, typename binop_t>
Node<T>* RBTree<T, binop_t>::remove_r( Node<T>* res )
{
    
    
    if( !res )
        return nullptr;

    // cout << "\nin a rec call of remove, obj: " << res->getObj() <<endl;


    Node<T>* temp = res->inOrderSuccessor();      //store to return one past the deleted node

    // cout << "Just after returning in order succ\n\n";
    // res->dispNode();
    // temp->dispNode();

    if(temp - res == 1)
    {    
        // if rightmost
        // cout << "rightmost: "<<res->getObj()<<endl;
        if( res->left == nullptr )
        {
            res->parent->right = nullptr;
            delete res;
        }

        return nullptr;
    }
    
    Node<T>* repl = BSTreplacement( *res );

    bool bothBlack = (repl == nullptr || repl->getColor() == BLACK) && (res->getColor() == BLACK);

    Node<T>* Parent = res->parent;

    // repl will be null only if it is a leaf
    if(repl == nullptr)
    {
        if(res == root)
            root = nullptr;
        
        else
        {
            // cout << res->getObj() << " is a leaf\n";

            if(bothBlack)
                correctTreeDelete(res);
            
            else
            {
                if(res->retSibling())
                    res->retSibling()->setColor(RED);
            }

            if(res->isOnLeft())
                Parent->left = nullptr;
            else
                Parent->right = nullptr;    
        }

        delete res;
        return temp;
        
    }
    
    // res can't be a leaf if the flow comes here
    if(res->right == nullptr || res->left == nullptr)
    {
        cout << res->getObj() << " is not a leaf\n";

        if(res == root)
        {
            res->setObj(repl->getObj());        //copy child's value into root or res
            res->left = res->right = nullptr;

            delete repl;
        }

        else
        {
            if(res->isOnLeft())
                Parent->left = repl;
            else
                Parent->right = repl;

            delete res;
            repl->parent = Parent;

            if(bothBlack)
                correctTreeDelete(repl);
            else
                repl->setColor(BLACK);
            
        }

        return temp;
        
    }
    
    // cout << res->getObj() << " <- swap -> " << repl->getObj() << "\n";
    swapValues(res, repl);
    
    return remove_r(repl);

}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::search(const T& obj)
{
    Node<T>* res = search_r(root, obj);

    if(!res)
        return end();

    return Iterator(res);   
}

template<typename T, typename binop_t>
Node<T>* RBTree<T, binop_t>::search_r(Node<T>* iter, const T& obj)
{
    // if( obj == iter->getObj() )
    if( !op( obj, iter->getObj() ) && !op( iter->getObj(), obj ) )
        return iter;

    if( !op( obj, iter->getObj() ) && iter->right != nullptr)
        return search_r(iter->right, obj);
    
    if( op(obj, iter->getObj() ) && iter->left != nullptr)
        return search_r(iter->left, obj);

    return nullptr;
}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::erase( const T& obj )
{
    // empty tree
    if( root == nullptr ) return end();

    Node<T>* res = search_r(root, obj);

    if(res != nullptr)
    {
        // try section
        Node<T>* inOrdSucc = res->inOrderSuccessor();
        // try section


        Node<T>* temp = remove_r( res );

        if( temp == nullptr)
            return end();
        
        Iterator dummy = Iterator(temp);

        if( inOrdSucc == temp )
        {
            // cout << "temp value inside if: " << temp->getObj() <<endl;
            return dummy;
        }
        // cout << "temp value outside if: " << temp->getObj() <<endl;
        return --dummy;

        
        // cout << "the root after erase operation: " << root->getObj() << "\n";
    }
    
    else
        return end();
}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::erase( RBTree<T, binop_t>::Iterator position )
{

    return erase(*position);

}

template<typename T, typename binop_t>
typename RBTree<T, binop_t>::Iterator RBTree<T, binop_t>::erase( RBTree<T, binop_t>::Iterator i1, RBTree<T, binop_t>::Iterator i2 )
{
    // cout << "Before loop i1 and i2 values: " << *i1 << *i2 <<endl;
    while(i1 != i2)
    {
        
        i1 = erase(*i1);
        // cout << "i1 value: " << *i1 << endl;

    }

    return i2;
}


#endif