#pragma once

#include <iostream>

// ChainNumber
// Author: Lokno Decker
//
// C++ Class for representing positive integers with linked lists
//
// Written as an exercise in absurty, but potentially useful 
// for memory obfuscation for calculations of small integers
//
// Most operations are O(n) where n is the number represented
// All equalities are O(min(a,b)) where a and b are two numbers being compared
// Multiplication and Division are O(n*m) where m is the second number represented
// Every copy stamp of ChainNumber completely recreates the list. O(n)
// Can represent any size integer, memory permitting 


class ChainNumber
{
    public:
    ChainNumber() { }

    ChainNumber( const ChainNumber &r )
    {
        *this = r;
    }

    ~ChainNumber()
    {
        reset();
    }

    // deletes the list, which resets the number to zero
    void reset()
    {
        while( this->m_head != NULL )
        {
            this->dec();
        }
    }

    // math function that computes base^power
    static ChainNumber pow( const ChainNumber &base, const ChainNumber &power )
    {
        ChainNumber new_num;

        new_num.inc();

        NumNode* curr = power.m_head;
        while( curr != NULL )
        {
            new_num *= base;
            curr = curr->m_next;
        }

        return new_num;      
    }

    // returns an object with a list of equivalent length 
    ChainNumber clone() const
    {
        ChainNumber new_num;

        NumNode* curr = this->m_head;
        while( curr != NULL )
        {
            new_num.inc();
            curr = curr->m_next;
        }

        return new_num;
    };

    // resizes list to match list on right-hand-side (rhs)
    // passes a reference to this object to the left of the operator
    ChainNumber& operator= (const ChainNumber &rhs)
    {
        // this->reset();
        // ChainNumber temp = rhs.clone();
        // this->m_head = temp.m_head;
        // temp.m_head = NULL;

        while( rhs > *this)
        {
            this->inc();
        }

        while( rhs < *this )
        {
            this->dec();
        }

        return *this;
    }

    // Returns a number representing the sum of the number
    // represented by this object and the rhs object
    ChainNumber operator+ (const ChainNumber &rhs)
    {
        ChainNumber result;

        NumNode* curr = this->m_head;
        while( curr != NULL )
        {
            result.inc();
            curr=curr->m_next;
        }
        curr = rhs.m_head;
        while( curr != NULL )
        {
            result.inc();
            curr=curr->m_next;
        }

        return result;
    };

    // Assigns this object to the sum of its 
    // current value the rhs object
    void operator+= (const ChainNumber& rhs)
    {
        NumNode* curr = rhs.m_head;
        while( curr != NULL )
        {
            this->inc();
            curr=curr->m_next;
        }
    }

    // Assigns this object to the product of its 
    // current value the rhs object
    void operator*= (const ChainNumber &rhs)
    {
        NumNode* curr = rhs.m_head;

        ChainNumber oldVal = this->clone();

        this->reset();

        while( curr != NULL )
        {
            curr = curr->m_next;
            *this += oldVal;
        }
    }

    // Returns a number representing the product of the number
    // represented by this object and the rhs object
    ChainNumber operator* (const ChainNumber &rhs)
    {
        ChainNumber result;
        NumNode* curr = rhs.m_head;

        while( curr != NULL )
        {
            curr = curr->m_next;
            result += *this;
        }

        return result;
    }

    // Assigns this object to the product of its 
    // current value the rhs object
    void operator/= (const ChainNumber &rhs)
    {
        ChainNumber result;

        while( !this->zero() )
        {
            if( *this != rhs )
            {
                *this -= rhs;
                if( !this->zero() ) result.inc();
            }
            else
            {
                result.inc();
                this->reset();
            }
        }

        *this = result;
    }

    // Returns a number representing the quotient of the number
    // represented by this object and the rhs object
    ChainNumber operator/ (const ChainNumber &rhs)
    {
        ChainNumber temp = this->clone();
        ChainNumber result;

        while( !temp.zero() )
        {
            if( temp != rhs )
            {
                temp -= rhs;
                if( !temp.zero() ) result.inc();
            }
            else
            {
                result.inc();
                temp.reset();
            }
        }

        return result;
    }

    // Returns a number representing the modulo of the number
    // represented by this object and the rhs object    
    ChainNumber operator% (const ChainNumber &rhs)
    {
        ChainNumber result = this->clone();

        while( result > rhs )
        {
            result -= rhs;
        }

        return result;
    }    

    // Returns a number representing the difference between the number
    // represented by this object and the rhs object
    // The difference is non-zero. 
    // max(0,a-b)
    ChainNumber operator- (const ChainNumber &rhs)
    {
        ChainNumber result = this->clone();
        NumNode* curr = rhs.m_head;

        while( curr != NULL && !result.zero() )
        {
            curr = curr->m_next;
            result.dec();
        }

        return result;
    };

    // Assigns this object to the difference between current number
    // represented by this object and the rhs object
    // The difference is non-zero. 
    // max(0,a-b)
    void operator-= (const ChainNumber& r)
    {
        NumNode* curr = r.m_head;

        while( curr != NULL && !this->zero() )
        {
            curr = curr->m_next;
            this->dec();
        } 
    }

    // An object with an empty list represents zero
    bool zero() { return this->m_head == NULL; };

    // Equalities 
    bool operator== (const ChainNumber &r) const
    { 
        NumNode *tl, *tr;
        advanceToFirstTail(r,&tl,&tr);
        return tl == tr; 
    };

    bool operator!= (const ChainNumber &r) const
    { 
        NumNode* tl, *tr;
        advanceToFirstTail(r,&tl,&tr);
        return tl != tr; 
    };

    bool operator< (const ChainNumber &r) const
    { 
        NumNode* tl, *tr;
        advanceToFirstTail(r,&tl,&tr);
        return tl == NULL && tr != NULL;  
    };

    bool operator> (const ChainNumber &r) const
    { 
        NumNode* tl, *tr;
        advanceToFirstTail(r,&tl,&tr);
        return tl != NULL && tr == NULL;  
    };

    bool operator<= (const ChainNumber &r) const
    { 
        NumNode* tl, *tr;
        advanceToFirstTail(r,&tl,&tr);
        return tl == NULL;  
    };

    bool operator>= (const ChainNumber &r) const
    { 
        NumNode* tl, *tr;
        advanceToFirstTail(r,&tl,&tr);
        return tr == NULL;  
    };

    ChainNumber& operator--()
    {
        this->dec();
        return *this;
    };

    ChainNumber& operator++()
    {
        this->inc();
        return *this;
    };

    ChainNumber operator--(int x)
    {
        this->dec();
        return *this;
    };

    ChainNumber operator++(int x)
    {
        this->inc();
        return *this;
    };

    // Increments the list and thus the represented integer
    void inc()
    {
        NumNode* temp = new NumNode();
        temp->m_next = this->m_head;
        this->m_head = temp;
    }

    // Decrements the list and thus the represented integer
    // in the case the list is empty nothing changes
    void dec()
    {
        if( this->m_head != NULL )
        {
            NumNode* temp = this->m_head;
            this->m_head = temp->m_next; 

            // Remove node from list and delete
            temp->m_next = NULL;
            delete temp;
        }   
    }

    // converts object to a size_t
    size_t toInt() const
    {
        size_t count = 0;
        NumNode* tail = this->m_head;
        while( tail != NULL )
        {
            tail = tail->m_next;
            count++;
        }        
        return count;
    }

    friend std::ostream& operator<<(std::ostream& os, const ChainNumber& rhs);

    private:
    // The node class used to form the linked list
    class NumNode
    {
    public:
        NumNode* m_next = NULL;
    };

    // The head of the linked list used to represent the ChainNumber value
    NumNode* m_head = NULL;

    // Advances the object's list in concert with the passed object's list
    // until the last node, or tail, of either list is discovered
    void advanceToFirstTail(const ChainNumber &rhs, NumNode** a, NumNode** b) const
    {
        *a = this->m_head;
        *b = rhs.m_head;

        while( *a != NULL && *b != NULL )
        {
            *a = (*a)->m_next;
            *b = (*b)->m_next;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const ChainNumber& rhs)
{
    os << rhs.toInt();
    return os;
}
