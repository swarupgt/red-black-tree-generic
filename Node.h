// Node class
enum COLOR{ RED, BLACK };


template<typename T>
class Node
{

    private:

        T obj_;
        COLOR color_;

    public:

        Node<T> *left;
        Node<T> *parent;
        Node<T> *right;

        Node() = delete;

        explicit Node( T );

        // getters and setters
        const COLOR& getColor();
        void setColor( COLOR );
        const T& getObj();
        void setObj( T );

        // other functions
        bool isOnLeft();
        Node<T> *retUncle();
        Node<T> *retSibling();

        void moveDown( Node<T>* );
        bool hasRedChild();
        
        Node<T>* inOrderSuccessor();
        Node<T>* inOrderPredecessor();
        
};

template<typename T>
Node<T>::Node( T obj ): obj_(obj), color_(RED), left(nullptr), parent(nullptr), right(nullptr)
{ }


template<typename T>
const COLOR& Node<T>::getColor()
{
    return color_;
}

template<typename T>
void Node<T>::setColor( COLOR color )
{
    color_ = color;
}


template<typename T>
const T& Node<T>::getObj()
{
    return obj_;
}

template<typename T>
void Node<T>::setObj( T obj )
{
    obj_ = obj;
}


template<typename T>
void Node<T>::moveDown( Node<T>* newParent )
{

    if ( parent != nullptr )
    {
      if ( this->isOnLeft() )
        parent->left = newParent;

      else
        parent->right = newParent;
    }
    
    newParent->parent = parent;
    parent = newParent;

}


template<typename T>
bool Node<T>::isOnLeft()
{
    if( parent == nullptr )
        return false;
    if( this == this->parent->left )
        return true;

    return false;
}

template<typename T>
Node<T>* Node<T>::retUncle()
{

    if( this->parent != nullptr && this->parent->parent != nullptr )
    {
        if( this->parent->isOnLeft() )
            return this->parent->parent->right;
        
        return this->parent->parent->left;
    }

    return nullptr;
}

template<typename T>
Node<T>* Node<T>::retSibling()
{
    if( parent != nullptr )
    {
        if( this == parent->left )
            return parent->right;

        if( this == parent->right )
            return parent->left;
    }

    return nullptr;
}

template<typename T>
bool Node<T>::hasRedChild()
{
    if( ( this->left != nullptr && this->left->getColor() == RED ) || ( this->right != nullptr && this->right->getColor() == RED ) )
        return true;
    return false;
}

template<typename T>
Node<T>* Node<T>::inOrderSuccessor()
{
    Node<T> *x = this;
    //case 1 gg
    if(x->right != nullptr)
    {
        x = x->right;
        while(x->left != nullptr)
        {
            x = x->left;
        }
        return x;   
    }

    //find root
    Node<T>* root = x;
    while( root->parent != nullptr )
        root = root->parent;

    //case 3 gg
    Node<T>* rmost = root;
    while (rmost->right != nullptr)
        rmost = rmost->right;
    
    if(x == rmost)
        return rmost+1;

    while(x!= x->parent->left)
        x = x->parent;
    
    return x->parent;

}

template<typename T>
Node<T>* Node<T>::inOrderPredecessor()
{
    Node<T> *x = this;
    if(x->left != nullptr)
    {
        x = x->left;
        while(x->right != nullptr)
        {
            x = x->right;
        }
        return x;   
    }

    //find root
    Node<T>* root = x;
    while( root->parent != nullptr )
        root = root->parent;

    Node<T>* leftMost = root;

    while (leftMost->left != nullptr)
        leftMost = leftMost->left;
    
    if(x == leftMost)
        return leftMost-1;

    while(x != x->parent->right)
        x = x->parent;
    return x->parent;

}
