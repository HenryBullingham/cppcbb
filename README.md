# Building Block C++ containters (cppcbb)

A lot of the time we want to customize containers for specific use cases by changing only small aspects of the container behaviour.
However, this usually means writing an entirely new container for just our use case. 
This library aims to provide several containers with a) flexible policies and b) opportunity to add just the policies you want for customization

Example: A vector with static storage, A vector with dynamic storage but O(1) delete and where order doesn't matter, A vector with static storage and O(1) delete

Current Containers:
  Vector
  Vector-backed Map

## Usage:

``` C++

#include "cppcbb/cbb_vector.h"

void main()
{
   //All of the vectors are based on the same core code, and use swappable components to customize their behaviour

    //Normal vector
    cppcbb::cbb_vector<int> regular_vector;
    //Can call push back, operator[] access, pop_back() and erase()

    //Static size vector
    cppcbb::cbb_static_vector<int, 50> static_vector;
    //Can call same methods as above

    //Vector where erase swaps with end element
    cppcbb:cbb_unordered_vector<int> unordered_vector;
    //Can call same methods as above

    //Static size vectorVector where erase swaps with end element
    cppcbb:cbb_static_unordered_vector<int, 50> static_unordered_vector;
    //Can call same methods as above
}


```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)