#ifndef TRIPLET_H
#define TRIPLET_H


template< typename First, typename Second, typename Third >
class Triplet
{
public:

    using FirstType = First;
    using SecondType = Second;
    using ThirdType = Third;

    Triplet()
        : first(),
          second(),
          third()
    {

    }

    Triplet(First f, Second s, Third t)
        : first(f),
          second(s),
          third(t)
    {

    }


    const First &getFirst() const { return first; }
    const Second &getSecond() const { return second; }
    const Third &getThird() const { return third; }

    First &getFirst() { return first; }
    Second &getSecond() { return second; }
    Third &getThird() { return third; }

private:
    First first;
    Second second;
    Third third;
};

#endif // TRIPLET_H
