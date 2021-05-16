#include <iostream>
#include <iterator>
#include <algorithm>

#include "RBTree.h"

using namespace std;

class point
{
    public:
        int x_;
        int y_;
        point(int x, int y): x_(x), y_(y) { }

        bool operator==( const point& rhs ) const
        {
            return x_==rhs.x_ && y_==rhs.y_;
        }

        bool operator!=( const point& rhs ) const
        {
            return !(*this == rhs);
        }

        friend ostream& operator<<( ostream& o, const point& x );
        
};

ostream& operator<<( ostream& o, const point& x )
{
    return o << "[" << x.x_ << ", " << x.y_ <<"]";
}

struct manDist
{
    bool operator() (point a, point b)
    {
        return a.x_ + a.y_ < b.x_ + b.y_;
    }
};

int main()
{

    // point a( 3,4 );
    // point b( 1,1 );
    // point c( 4,5 );
    
    point arr[] = { point( 8, 3 ),
                    point( 4, 1 ),
                    point( 12, 6 ), 
                    point( 2, 1 ), 
                    point( 6, 7 ),
                    point( 10, 4 ),
                    point( 14, 2 ),
                    point( 1, 3 ),
                    point( 3, 3 ),
                    point( 5, 6 ),
                    point( 7, 4 ),
                    point( 9, 7 ),
                    point( 11, 1 ),
                    point( 13, 4 ),
                    point( 15, 0 ) };

    cout << boolalpha;
    RBTree<point, manDist> tree;
    
    cout << boolalpha;
    
    // tree.insert(a);
    // tree.insert(b);
    // tree.insert(c);

    // int arr[] = {8, 4, 12 ,2, 6 ,10, 14, 1, 3, 5, 7, 9, 11, 13, 15};
    int size = sizeof( arr ) / sizeof( arr[0] );

    for (int i = 0; i < size; ++i)
        tree.insert(arr[i]);

    //tree.disp();

    tree.erase(point(15,2));
    
    tree.disp2d();

    // tree.erase(15);

    // tree.disp();

    // auto x = find(tree.begin(), tree.end(), 10);
    // if( x != tree.end() )
    //     cout << "find returns " << *x << endl;

    
    return 0;
}