


#include <exception>
#include <string>
#include <iostream>

#include <cstdio>
#include <cstdarg>

#include "Eagle/StringWork.hpp"
#include "Eagle/Error.hpp"

/*

std::string StringPrintF(const char* format_str , ...) {
   char buffer[1025];
   va_list args;
   va_start(args , format_str);
   vsprintf(buffer , format_str , args);
   va_end(args);
   return std::string(buffer);
}



class Error : public std::exception {
private :
   std::string e;
public :
   Error(const std::string& error) : e(error) {
#ifdef DEBUG
      std::cout << error << std::endl;
#endif
   }
   Error(const char* error) : e(error) {
#ifdef DEBUG
      std::cout << error << std::endl;
#endif
   }
   ~Error() throw() {}

   const char* what() const throw () {return e.c_str();}
};
*/


/// One dimensional array, based on placement new, uses Type's copy constructor when allocating elements.
/// All operations resize (reallocate and initialize memory). Useful for arrays that don't change size too often.
template <class Type>
class Array {
private :
   void* mem;
   void* alignmem;
   Type* array;
   unsigned int size;

   void* Allocate(unsigned int new_size , void** store_alignedmem);

public :
   Array();
   Array(unsigned int n , const Type& init_data);
   Array(const Array& a);

   Array& operator=(const Array& a);
   
   ~Array() {Free();}

   void Free();

   void Resize(unsigned int n , const Type& init_data , bool preserve);

   void ClearTo(const Type& data);

   void InsertBefore(unsigned int index , const Type& data);
   void InsertAfter(unsigned int index , const Type& data);

   void Erase(unsigned int index);

   void PushFront(const Type& data);
   void PushBack(const Type& data);

   void PopFront();
   void PopBack();

   Type& At(unsigned int index);
   const Type& At(unsigned int index) const;
   
   Type& operator[] (unsigned int index);
   const Type& operator[] (unsigned int index) const;
   
   const unsigned int Size() const {return size;}
};



template <class Type , size_t num_dimensions>
class MultiArray {

   typedef MultiArray<Type , num_dimensions - 1> SubArray;

private :
   Array<SubArray> array;

public :
   friend class Iterator;
   friend class ConstIterator;
   
   class Iterator {
   private :
      MultiArray<Type , num_dimensions>* m;
      Array<SubArray>* a;
      Type* t;
      unsigned int i;// index
      typename MultiArray<Type,num_dimensions -1>::Iterator it;
   public :
      friend class MultiArray<Type,num_dimensions>::ConstIterator;
   
      Iterator();
      Iterator(MultiArray<Type , num_dimensions>* multiarray , unsigned int index);

      bool operator==(const Iterator& iter);
      bool operator!=(const Iterator& iter) {return !(*this == iter);}

      Iterator& operator++();
      Iterator& operator--();

      Iterator operator++(int);
      Iterator operator--(int);

      
      Type& operator*();
   };

   class ConstIterator {
   private :
      const MultiArray<Type , num_dimensions>* m;
      const Array<SubArray>* a;
      const Type* t;
      unsigned int i;// index
      typename MultiArray<Type,num_dimensions -1>::ConstIterator it;
   public :
      ConstIterator();
      ConstIterator(const MultiArray<Type , num_dimensions>* multiarray , unsigned int index);
      ConstIterator(const Iterator& iter);

      ConstIterator& operator=(const Iterator& iter);

      bool operator==(const ConstIterator& iter);
      bool operator!=(const ConstIterator& iter) {return !(*this == iter);}

      bool operator==(const MultiArray<Type,num_dimensions>::Iterator& iter);
      bool operator!=(const MultiArray<Type,num_dimensions>::Iterator& iter) {return !(*this == iter);}

      ConstIterator& operator++();
      ConstIterator& operator--();
      
      ConstIterator operator++(int);
      ConstIterator operator--(int);
      
      const Type& operator*();
   };

   MultiArray();
   MultiArray(const unsigned int* new_sizes , const Type& data);
   MultiArray(const MultiArray& a);

   MultiArray& operator=(const MultiArray& a);

   void StoreSizes(unsigned int* store_sizes);

   void Free();

   void Resize(const unsigned int* new_sizes , const Type& data , bool preserve);

   void ClearTo(const Type& data);

   void InsertBefore(unsigned int index , const MultiArray<Type , num_dimensions - 1>& data);
   void InsertAfter(unsigned int index , const MultiArray<Type , num_dimensions - 1>& data);

   void Erase(unsigned int index);

   void PushFront(const MultiArray<Type , num_dimensions - 1>& data);
   void PushBack(const MultiArray<Type , num_dimensions - 1>& data);

   void PopFront();
   void PopBack();

   MultiArray<Type , num_dimensions - 1>& At(unsigned int index);
   const MultiArray<Type , num_dimensions - 1>& At(unsigned int index) const ;

   MultiArray<Type , num_dimensions - 1>& operator[](unsigned int index) {return array[index];}
   const MultiArray<Type , num_dimensions - 1>& operator[](unsigned int index) const {return array[index];}
   
   Iterator Begin();
   Iterator Index(unsigned int i);
   Iterator Last() {return --(End());}
   Iterator End();
   
   ConstIterator Begin() const;
   ConstIterator Index(unsigned int i) const;
   ConstIterator Last() const {return --(End());}
   ConstIterator End() const;

   const unsigned int TotalSize();
   const unsigned int Size() {return array.Size();}
};



template <>
template <class Type>
class MultiArray<Type , 1> {
private :
   Array<Type> array;

public :
   friend class Iterator;
   
   class Iterator {
   private :
      MultiArray<Type , 1>* m;
      Array<Type>* a;
      Type* t;
      unsigned int i;
   public :
      friend class MultiArray<Type,1>::ConstIterator;
      
      Iterator();
      Iterator(MultiArray<Type , 1>* multiarray , unsigned int index);

      bool operator==(const Iterator& iter);
      bool operator!=(const Iterator& iter) {return !(*this == iter);}

      Iterator& operator++();
      Iterator& operator--();
      
      Iterator operator++(int);
      Iterator operator--(int);
      
      Type& operator*();
   };

   class ConstIterator {
   private :
      const MultiArray<Type , 1>* m;
      const Array<Type>* a;
      const Type* t;
      unsigned int i;
   public :
      ConstIterator();
      ConstIterator(const MultiArray<Type , 1>* multiarray , unsigned int index);
      ConstIterator(const Iterator& iter);

      ConstIterator& operator=(const Iterator& iter);

      bool operator==(const MultiArray<Type,1>::ConstIterator& iter);
      bool operator!=(const MultiArray<Type,1>::ConstIterator& iter) {return !(*this == iter);}

      bool operator==(const Iterator& iter);
      bool operator!=(const Iterator& iter) {return !(*this == iter);}

      ConstIterator& operator++();
      ConstIterator& operator--();
      
      ConstIterator operator++(int);
      ConstIterator operator--(int);
      
      const Type& operator*();
   };

   MultiArray();
   MultiArray(const unsigned int* new_sizes , const Type& data);
   MultiArray(const MultiArray& a);

   MultiArray& operator=(const MultiArray& a);

   void StoreSizes(unsigned int* store_sizes);

   void Free();

   void Resize(const unsigned int* new_sizes , const Type& data , bool preserve);
   
   void ClearTo(const Type& data);

   void InsertBefore(unsigned int index , const Type& data);
   void InsertAfter(unsigned int index , const Type& data);

   void Erase(unsigned int index);

   void PushFront(const Type& data);
   void PushBack(const Type& data);

   void PopFront();
   void PopBack();

   Type& At(unsigned int index);
   const Type& At(unsigned int index) const ;

   Type& operator[](unsigned int index) {return array[index];}
   const Type& operator[](unsigned int index) const {return array[index];}
   
   Iterator Begin();
   Iterator Index(unsigned int index);
   Iterator Last() {return --(End());}
   Iterator End();
   
   ConstIterator Begin() const ;
   ConstIterator Index(unsigned int index) const ;
   ConstIterator Last() const {return --(End());}
   ConstIterator End() const ;

   const unsigned int TotalSize() {return array.Size();}
   const unsigned int Size() {return array.Size();}
};



template <>
template <class Type>
class MultiArray<Type , 0> {
private :
   MultiArray() {throw std::exception("Can't declare a MultiArray<Type,0> object!!!");}
};




/// ---------------------------------     Array class     ---------------------------------



template <class Type>
void* Array<Type>::Allocate(unsigned int new_size , void** store_alignedmem) {
   void* rawmem = malloc(new_size*sizeof(Type) + 7);// allocate 7 more bytes than necessary to allow for alignment fix
   unsigned int align = 0;
   unsigned int misalign = (unsigned int)rawmem % 8;
   if (misalign) {
      align = 8 - misalign;
   }
   void* alignedmem = (void*)((char*)rawmem + align);
   *store_alignedmem = alignedmem;
   return rawmem;
}



template <class Type>
Array<Type>::Array() :
      mem(0),
      alignmem(0),
      array(0),
      size(0)
{}



template <class Type>
Array<Type>::Array(unsigned int n , const Type& init_data) :
      mem(0),
      alignmem(0),
      array(0),
      size(0)
{
   Resize(n,init_data,false);
}



template <class Type>
Array<Type>::Array(const Array& a) :
      mem(0),
      alignmem(0),
      array(0),
      size(0)
{
   if (!a.Size()) {return;}
   void* newmem = 0;
   void* newalignmem = 0;
   Type* newarray = 0;
   unsigned int newsize = a.Size();
   
   newmem = Allocate(newsize , &newalignmem);
   if (!newmem) {throw std::bad_alloc();}
   
   newarray = (Type*)newalignmem;
   Type* element = newarray;
   unsigned int x = 0;
   try {
      for (x = 0 ; x < newsize ; ++x) {
         new ((void*)element) Type(a[x]);
         ++element;
      }
   }
   catch (...) {
      if (x) {
         for (unsigned int x2 = x - 1 ; ; --x2) {
            newarray[x2].~Type();
            if (x2 == 0) {break;}
         }
      }
      free(newmem);
      throw;
   }
   mem = newmem;
   alignmem = newalignmem;
   array = newarray;
   size = newsize;
}



template <class Type>
Array<Type>& Array<Type>::operator=(const Array<Type>& a) {
   if (&a == this) {return *this;}
   if (!a.Size()) {
      Free();
      return *this;
   }
   if (size == a.size) {
      try {
         for (unsigned int x = 0 ; x < size ; ++x) {
            array[x] = a.array[x];
         }
      }
      catch (...) {
         /// TODO : What should we do if assignment fails? Leave the objects as they are?
         /// The state of the objects already assigned is unrecoverable.
         throw;
      }
      return *this;
   }
   void* newmem = 0;
   void* newalignmem = 0;
   Type* newarray = 0;
   unsigned int newsize = a.Size();
   
   newmem = Allocate(newsize , &newalignmem);
   if (!newmem) {throw std::bad_alloc();}
   
   newarray = (Type*)newalignmem;
   Type* element = newarray;
   unsigned int x = 0;
   try {
      for (x = 0 ; x < newsize ; ++x) {
         new ((void*)element) Type(a[x]);
         ++element;
      }
   }
   catch (...) {
      if (x) {
         for (unsigned int x2 = x - 1 ; ; --x2) {
            newarray[x2].~Type();
            if (x2 == 0) {break;}
         }
      }
      free(newmem);
      throw;
   }
   Free();
   mem = newmem;
   alignmem = newalignmem;
   array = newarray;
   size = newsize;
   
   return *this;
}



template <class Type>
void Array<Type>::Free() {
   if (array) {
      if (size) {
         for (unsigned int x = size - 1 ; ; --x) {
            array[x].~Type();
            if (x == 0) {break;}
         }
      }
      free(mem);
      mem = 0;
      array = 0;
      size = 0;
   }
}



template <class Type>
void Array<Type>::Resize(unsigned int n , const Type& init_data , bool preserve) {
   if (n == size) {return;}// nothing to do
   if (n == 0) {
      Free();
      return;
   }

   void* newmem = 0;
   void* newalignmem = 0;
   Type* newarray = 0;
   unsigned int newsize = n;

   if (!preserve) {
      Free();
   }
   
   newmem = Allocate(newsize , &newalignmem);
   if (!newmem) {throw std::bad_alloc();}
   newarray = (Type*)newalignmem;
   
   if (!preserve) {
      unsigned int x = 0;
      Type* element = newarray;
      try {
         for (x = 0 ; x < newsize ; ++x) {
            new ((void*)element) Type(init_data);
            ++element;
         }
      }
      catch (...) {
         if (x) {
            for (unsigned int x2 = x - 1 ; ; --x2) {
               newarray[x2].~Type();
               if (x2 == 0) {break;}
            }
         }
         free(newmem);
         throw;
      }
      
   } else {
      unsigned int minsize = (size < newsize)?size:newsize;
      Type* element = newarray;
      unsigned int x = 0;
      try {
         for (x = 0 ; x < minsize ; ++x) {
            new ((void*)element) Type(array[x]);
            ++element;
         }
         for (x = minsize ; x < newsize ; ++x) {
            new ((void*)element) Type(init_data);
            ++element;
         }
      }
      catch (...) {
         if (x) {
            for (unsigned int x2 = x - 1 ; ; --x2) {
               newarray[x2].~Type();
               if (x2 == 0) {break;}
            }
         }
         free(newmem);
         throw;
      }
      Free();
   }
   mem = newmem;
   alignmem = newalignmem;
   array = newarray;
   size = newsize;
}



template <class Type>
void Array<Type>::ClearTo(const Type& data) {
   for (unsigned int i = 0 ; i < size ; ++i) {
      array[i] = data;
   }
}



template <class Type>
void Array<Type>::InsertBefore(unsigned int index , const Type& data) {
   if (index > size) {
      throw EagleError(StringPrintF("Array::InsertBefore - Trying to insert data out of bounds(index = %u , size = %u).", index , size));
   }
   
   void* newmem = 0;
   void* newalignmem = 0;
   Type* newarray = 0;
   unsigned int newsize = size + 1;
   
   newmem = Allocate(newsize , &newalignmem);
   if (!newmem) {throw std::bad_alloc();}
   
   newarray = (Type*)newalignmem;

   Type* element = newarray;
   unsigned int x = 0;
   try {
      for (x = 0 ; x < index ; ++x) {
         new ((void*)element) Type(array[x]);
         ++element;
      }
      new ((void*)element) Type(data);
      ++x;
      ++element;
      for ( ; x < newsize ; ++x) {
         new ((void*)element) Type(array[x - 1]);
         ++element;
      }
   }
   catch (...) {
      if (x) {
         for (unsigned int x2 = x - 1 ; ; --x2) {
            newarray[x2].~Type();
            if (x2 == 0) {break;}
         }
      }
      free(newmem);
      throw;
   }
   Free();
   mem = newmem;
   alignmem = newalignmem;
   array = newarray;
   size = newsize;
}



template <class Type>
void Array<Type>::InsertAfter(unsigned int index , const Type& data) {
   if (size == 0) {InsertBefore(0,data);return;}
   if (index >= size) {throw EagleError("Array::InsertAfter - index out of bounds");}
   InsertBefore(index + 1 , data);
}



template <class Type>
void Array<Type>::Erase(unsigned int index) {
   if (index >= size) {
      throw EagleError(StringPrintF("Array::Erase - index(%u) out of bounds [0,%u)" , index , size));
   }
   
   void* newmem = 0;
   void* newalignmem = 0;
   Type* newarray = 0;
   unsigned int newsize = size - 1;
   
   newmem = Allocate(newsize , &newalignmem);
   if (!newmem) {throw std::bad_alloc();}
   
   newarray = (Type*)newalignmem;
   
   unsigned int x = 0;
   Type* element = newarray;
   try {
      for (x = 0 ; x < index ; ++x) {
         new ((void*)element) Type(array[x]);
         ++element;
      }
      // erase this element by skipping it
      for (x = index ; x < newsize ; ++x) {
         new ((void*)element) Type(array[x + 1]);
         ++element;
      }
   }
   catch (...) {
      if (x) {
         for (unsigned int x2 = x - 1 ; ; --x2) {
            newarray[x2].~Type();
            if (x2 == 0) {break;}
         }
      }
      free(newmem);
      throw;
   }
   Free();
   mem = newmem;
   alignmem = newalignmem;
   array = newarray;
   size = newsize;
}



template <class Type>
void Array<Type>::PushFront(const Type& data) {
   InsertBefore(0,data);
}



template <class Type>
void Array<Type>::PushBack(const Type& data) {
   InsertBefore(size , data);
}



template <class Type>
void Array<Type>::PopFront() {
   if (size) {
      Erase(0);
   } else {
      throw EagleError("Array::PopFront - trying to pop an empty array.");
   }
}



template <class Type>
void Array<Type>::PopBack() {
   if (size) {
      Erase(size - 1);
   } else {
      throw EagleError("Array::PopBack - trying to pop an empty array.");
   }
}



template <class Type>
Type& Array<Type>::At(unsigned int index) {
   if (index >= size) {
      throw EagleError(StringPrintF("Array::At - index (%u) out of bounds [0,%u)." , index , size));
   }
   return array[index];
}



template <class Type>
const Type& Array<Type>::At(unsigned int index) const {
   if (index >= size) {
      throw EagleError(StringPrintF("Array::At - index (%u) out of bounds [0,%u)." , index , size));
   }
   return array[index];
}



template <class Type>
Type& Array<Type>::operator[] (unsigned int index) {
   return array[index];
}



template <class Type>
const Type& Array<Type>::operator[] (unsigned int index) const {
   return array[index];
}




/// ----------------------- MultiArray<Type , n> -----------------------------------



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::MultiArray() :
      array()
{}



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::MultiArray(const unsigned int* new_sizes , const Type& data) :
      array()
{
   if (!new_sizes) {throw EagleError(StringPrintF("MultiArray<Type , &u>::MultiArray - new_sizes is NULL!" , num_dimensions));}
   try {
      Resize(new_sizes , data , false);
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::MultiArray(const MultiArray<Type , num_dimensions>& a) :
      array()
{
   try {
      array = a.array;
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>& MultiArray<Type , num_dimensions>::operator=(const MultiArray<Type , num_dimensions>& a) {
   if (&a == this) {return *this;}
   try {
      array = a.array;
   }
   catch (...) {
      throw;
   }
   return *this;
}



template <class Type , size_t num_dimensions>
void MultiArray<Type , num_dimensions>::StoreSizes(unsigned int* store_sizes) {
   if (!store_sizes) {
      throw EagleError(StringPrintF("MultiArray<Type , %u>::StoreSizes - store_sizes pointer is NULL!\n" , num_dimensions));
   }
   store_sizes[0] = Size();
   if (Size()) {
      array[0].StoreSizes(&store_sizes[1]);
   } else {
      for (unsigned int i = 1 ; i < num_dimensions ; ++i) {
         store_sizes[i] = 0;
      }
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::Free() {
   array.Free();
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::Resize(const unsigned int* new_sizes , const Type& data , bool preserve) {
   if (!new_sizes) {
      throw EagleError(StringPrintF("MultiArray<Type , %u>::Resize - new_sizes pointer is NULL!" , num_dimensions));
   }

//   bool same_size = true;
//   for (unsigned int i = 0 ; i < num_dimensions ; ++i) {
//      if (sizes[i] != new_sizes[i]) {
//         same_size = false;
//         break;
//      }
//   }
//   if (same_size) {return;}

   try {
      array.Resize(new_sizes[0] , SubArray(&new_sizes[1] , data) , preserve);
      for (unsigned int i = 0 ; i < array.Size() ; ++i) {
         SubArray& a = array[i];
         a.Resize(&new_sizes[1] , data , preserve);
      }
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type , num_dimensions>::ClearTo(const Type& data) {
   for (unsigned int i = 0 ; i < array.Size() ; ++i) {
      array[i].ClearTo(data);
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::InsertBefore(unsigned int index , const MultiArray<Type , num_dimensions - 1>& data) {
   try {
      array.InsertBefore(index , data);
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::InsertAfter(unsigned int index , const MultiArray<Type , num_dimensions - 1>& data) {
   try {
      array.InsertAfter(index , data);
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::Erase(unsigned int index) {
   try {
      array.Erase(index);
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::PushFront(const MultiArray<Type , num_dimensions - 1>& data) {
   try {
      array.PushFront(data);
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::PushBack(const MultiArray<Type , num_dimensions - 1>& data) {
   try {
      array.PushBack(data);
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::PopFront() {
   try {
      array.PopFront();
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
void MultiArray<Type,num_dimensions>::PopBack() {
   try {
      array.PopBack();
   }
   catch (...) {
      throw;
   }
}



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions - 1>& MultiArray<Type,num_dimensions>::At(unsigned int index) {
   return array.At(index);
}



template <class Type , size_t num_dimensions>
const MultiArray<Type , num_dimensions - 1>& MultiArray<Type,num_dimensions>::At(unsigned int index) const {
   return array.At(index);
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::Iterator MultiArray<Type,num_dimensions>::Begin() {
   return Iterator(this , 0);
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::Iterator MultiArray<Type,num_dimensions>::Index(unsigned int i) {
   return Iterator(this , i);
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::Iterator MultiArray<Type,num_dimensions>::End() {
   return Iterator(this , array.Size());
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::ConstIterator MultiArray<Type,num_dimensions>::Begin() const {
   return ConstIterator(this , 0);
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::ConstIterator MultiArray<Type,num_dimensions>::Index(unsigned int i) const {
   return ConstIterator(this , i);
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::ConstIterator MultiArray<Type,num_dimensions>::End() const {
   return ConstIterator(this , array.Size());
}



template <class Type , size_t num_dimensions>
const unsigned int MultiArray<Type , num_dimensions>::TotalSize() {
   unsigned int sizes[num_dimensions];
   StoreSizes(sizes);
   unsigned int totalsize = 1;
   for (unsigned int i = 0 ; i < num_dimensions ; ++i) {
      totalsize *= sizes[i];
   }
   return totalsize;
}



/// --------------------------   MultiArray<Type , n>::Iterator    ---------------------------------------------



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::Iterator::Iterator() :
      m(0),
      a(0),
      t(0),
      i(0),
      it()
{}



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::Iterator::Iterator(MultiArray<Type , num_dimensions>* multiarray , unsigned int index) :
      m(0),
      a(0),
      t(0),
      i(0),
      it()
{
   if (!multiarray) {return;}
   m = multiarray;
   a = &(m->array);
   if (index >= a->Size()) {
      i = a->Size();
      return;
   }
   i = index;
   it = (*a)[i].Begin();
   t = &(*it);
}



template <class Type , size_t num_dimensions>
bool MultiArray<Type , num_dimensions>::Iterator::operator==(const MultiArray<Type,num_dimensions>::Iterator& iter) {
   return ((m == iter.m) &&
           (a == iter.a) &&
           (t == iter.t) &&
           (i == iter.i));
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type , num_dimensions>::Iterator& MultiArray<Type , num_dimensions>::Iterator::operator++() {
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++it;
   if (it == (*a)[i].End()) {
      ++i;
      if (i == a->Size()) {
         t = 0;
         if (*this != m->End()) {
            throw EagleError(StringPrintF("MultiArray<Type , %u>::Iterator::operator++ - Reached end of array but not equal to End()!" , num_dimensions));
         }
      } else {
         it = (*a)[i].Begin();
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   t = &(*it);
   return *this;
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type , num_dimensions>::Iterator& MultiArray<Type , num_dimensions>::Iterator::operator--() {
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {// at end, move to last element
      if (a->Size()) {
         --i;
         it = --((*a)[i].End());
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   --it;
   if (it == (*a)[i].End()) {
      --i;
      it = --((*a)[i].End());
      if (it != (*a)[i].End()) {
         t = &(*it);
      } else {
         t = 0;
      }
      return *this;
   }
   t = &(*it);
   return *this;
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::Iterator MultiArray<Type,num_dimensions>::Iterator::operator++(int) {
   Iterator iter = *this;
   ++(*this);
   return iter;
/** This doesn't return the same iterator that was passed to it
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++it;
   if (it == (*a)[i].End()) {
      ++i;
      if (i == a->Size()) {
         t = 0;
         if (*this != m->End()) {
            throw EagleError(StringPrintF("MultiArray<Type , %u>::Iterator::operator++ - Reached end of array but not equal to End()!" , num_dimensions));
         }
      } else {
         it = (*a)[i].Begin();
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   t = &(*it);
   return *this;
*/
}
template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::Iterator MultiArray<Type,num_dimensions>::Iterator::operator--(int) {
   Iterator iter = *this;
   --(*this);
   return iter;
/** This doesn't return the same iterator that was passed to it
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {// at end, move to last element
      if (a->Size()) {
         --i;
         it = --((*a)[i].End());
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   --it;
   if (it == (*a)[i].End()) {
      --i;
      it = --((*a)[i].End());
      if (it != (*a)[i].End()) {
         t = &(*it);
      } else {
         t = 0;
      }
      return *this;
   }
   t = &(*it);
   return *this;
*/
}



template <class Type , size_t num_dimensions>
Type& MultiArray<Type , num_dimensions>::Iterator::operator*() {
   return *t;
}



/// --------------------------    MultiArray<Type , n>::ConstIterator    ---------------------------------



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::ConstIterator::ConstIterator() :
      m(0),
      a(0),
      t(0),
      i(0),
      it()
{}



template <class Type , size_t num_dimensions>
MultiArray<Type , num_dimensions>::ConstIterator::ConstIterator(const MultiArray<Type , num_dimensions>* multiarray , unsigned int index) :
      m(0),
      a(0),
      t(0),
      i(0),
      it()
{
   if (!multiarray) {return;}
   m = multiarray;
   a = &(m->array);
   if (index >= a->Size()) {
      i = a->Size();
      return;
   }
   i = index;
   it = (*a)[i].Begin();
   t = &(*it);
}



template <class Type , size_t num_dimensions>
MultiArray<Type,num_dimensions>::ConstIterator::ConstIterator(const MultiArray<Type,num_dimensions>::Iterator& iter) :
      m(iter.m),
      a(iter.a),
      t(iter.t),
      i(iter.i),
      it(iter.it)
{}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::ConstIterator& MultiArray<Type , num_dimensions>::ConstIterator::operator=(const MultiArray<Type,num_dimensions>::Iterator& iter) {
   m = iter.m;
   a = iter.a;
   t = iter.t;
   i = iter.i;
   it = iter.it;
   return *this;
}



template <class Type , size_t num_dimensions>
bool MultiArray<Type , num_dimensions>::ConstIterator::operator==(const MultiArray<Type,num_dimensions>::ConstIterator& iter) {
   return ((m == iter.m) &&
           (a == iter.a) &&
           (t == iter.t) &&
           (i == iter.i));
}



template <class Type , size_t num_dimensions>
bool MultiArray<Type,num_dimensions>::ConstIterator::operator==(const MultiArray<Type,num_dimensions>::Iterator& iter) {
   return ((m == iter.m) &&
           (a == iter.a) &&
           (t == iter.t) &&
           (i == iter.i));
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type , num_dimensions>::ConstIterator& MultiArray<Type , num_dimensions>::ConstIterator::operator++() {
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++it;
   if (it == (*a)[i].End()) {
      ++i;
      if (i == a->Size()) {
         t = 0;
         if (*this != m->End()) {
            throw EagleError(StringPrintF("MultiArray<Type , %u>::ConstIterator::operator++ - Reached end of array but not equal to End()!" , num_dimensions));
         }
      } else {
         it = (*a)[i].Begin();
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   t = &(*it);
   return *this;
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type , num_dimensions>::ConstIterator& MultiArray<Type , num_dimensions>::ConstIterator::operator--() {
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {// at end, move to last element
      if (a->Size()) {
         --i;
         it = --((*a)[i].End());
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   --it;
   if (it == (*a)[i].End()) {
      --i;
      it = --((*a)[i].End());
      if (it != (*a)[i].End()) {
         t = &(*it);
      } else {
         t = 0;
      }
      return *this;
   }
   t = &(*it);
   return *this;
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::ConstIterator MultiArray<Type,num_dimensions>::ConstIterator::operator++(int) {
   ConstIterator iter = *this;
   ++(*this);
   return iter;
/** This doesn't return the same iterator that was passed to it
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++it;
   if (it == (*a)[i].End()) {
      ++i;
      if (i == a->Size()) {
         t = 0;
         if (*this != m->End()) {
            throw EagleError(StringPrintF("MultiArray<Type , %u>::ConstIterator::operator++ - Reached end of array but not equal to End()!" , num_dimensions));
         }
      } else {
         it = (*a)[i].Begin();
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   t = &(*it);
   return *this;
*/
}



template <class Type , size_t num_dimensions>
typename MultiArray<Type,num_dimensions>::ConstIterator MultiArray<Type,num_dimensions>::ConstIterator::operator--(int) {
   ConstIterator iter = *this;
   --(*this);
   return iter;
/** This doesn't return the iterator that was passed to it
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {// at end, move to last element
      if (a->Size()) {
         --i;
         it = --((*a)[i].End());
         if (it != (*a)[i].End()) {
            t = &(*it);
         } else {
            t = 0;
         }
      }
      return *this;
   }
   --it;
   if (it == (*a)[i].End()) {
      --i;
      it = --((*a)[i].End());
      if (it != (*a)[i].End()) {
         t = &(*it);
      } else {
         t = 0;
      }
      return *this;
   }
   t = &(*it);
   return *this;
*/
}



template <class Type , size_t num_dimensions>
const Type& MultiArray<Type , num_dimensions>::ConstIterator::operator*() {
   return *t;
}


/// ---------------------------- MultiArray<Type , 1> ----------------------------------------------



template <>
template <class Type>
MultiArray<Type , 1>::MultiArray() :
      array()
{}



template <>
template <class Type>
MultiArray<Type , 1>::MultiArray(const unsigned int* new_sizes , const Type& data) :
      array()
{
   if (!new_sizes) {throw EagleError("MultiArray<Type , 1>::MultiArray - new_sizes is NULL!");}
   try {
      array.Resize(new_sizes[0] , data , false);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
MultiArray<Type , 1>::MultiArray(const MultiArray<Type , 1>& a) :
      array()
{
   try {
      array = a.array;
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
MultiArray<Type , 1>& MultiArray<Type , 1>::operator=(const MultiArray<Type , 1>& a) {
   if (&a == this) {return *this;}
   try {
      array = a.array;
   }
   catch (...) {
      throw;
   }
   return *this;
}



template <>
template <class Type>
void MultiArray<Type , 1>::StoreSizes(unsigned int* store_sizes) {
   if (!store_sizes) {
      throw EagleError("MultiArray<Type , 1>::StoreSizes - store_sizes pointer is NULL!\n");
   }
   if (store_sizes) {
      store_sizes[0] = Size();
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::Free() {
   array.Free();
}



template <>
template <class Type>
void MultiArray<Type , 1>::Resize(const unsigned int* new_sizes , const Type& data , bool preserve) {
   if (!new_sizes) {
      throw EagleError("MultiArray<Type , 1>::Resize - new_sizes is NULL");
   }
   try {
      array.Resize(new_sizes[0] , data , preserve);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type , 1>::ClearTo(const Type& data) {
   array.ClearTo(data);
}


template <>
template <class Type>
void MultiArray<Type,1>::InsertBefore(unsigned int index , const Type& data) {
   try {
      array.InsertBefore(index , data);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::InsertAfter(unsigned int index , const Type& data) {
   try {
      array.InsertAfter(index , data);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::Erase(unsigned int index) {
   try {
      array.Erase(index);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::PushFront(const Type& data) {
   try {
      array.PushFront(data);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::PushBack(const Type& data) {
   try {
      array.PushBack(data);
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::PopFront() {
   try {
      array.PopFront();
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
void MultiArray<Type,1>::PopBack() {
   try {
      array.PopBack();
   }
   catch (...) {
      throw;
   }
}



template <>
template <class Type>
Type& MultiArray<Type,1>::At(unsigned int index) {
   return array.At(index);
}



template <>
template <class Type>
const Type& MultiArray<Type,1>::At(unsigned int index) const {
   return array.At(index);
}



template <>
template <class Type>
typename MultiArray<Type,1>::Iterator MultiArray<Type,1>::Begin() {
   return Iterator(this , 0);
}



template <>
template <class Type>
typename MultiArray<Type,1>::Iterator MultiArray<Type,1>::Index(unsigned int index) {
   return Iterator(this , index);
}



template <>
template <class Type>
typename MultiArray<Type,1>::Iterator MultiArray<Type,1>::End() {
   return Iterator(this , array.Size());
}



template <>
template <class Type>
typename MultiArray<Type,1>::ConstIterator MultiArray<Type,1>::Begin() const {
   return ConstIterator(this , 0);
}



template <>
template <class Type>
typename MultiArray<Type,1>::ConstIterator MultiArray<Type,1>::Index(unsigned int index) const {
   return ConstIterator(this , index);
}



template <>
template <class Type>
typename MultiArray<Type,1>::ConstIterator MultiArray<Type,1>::End() const {
   return ConstIterator(this , array.Size());
}



/// -----------------------    MultiArray<Type,1>::Iterator     -------------------------------------



template <>
template <class Type>
MultiArray<Type,1>::Iterator::Iterator() :
      m(0),
      a(0),
      t(0),
      i(0)
{}



template <>
template <class Type>
MultiArray<Type,1>::Iterator::Iterator(MultiArray<Type , 1>* multiarray , unsigned int index) :
      m(0),
      a(0),
      t(0),
      i(0)
{
   if (!multiarray) {return;}
   m = multiarray;
   a = &(m->array);
   if (index >= a->Size()) {
      i = a->Size();
      return;
   }
   i = index;
   t = &((*a)[i]);
}



template <>
template <class Type>
bool MultiArray<Type,1>::Iterator::operator==(const MultiArray<Type,1>::Iterator& iter) {
   return ((m == iter.m) &&
           (a == iter.a) &&
           (t == iter.t) &&
           (i == iter.i));
}



template <>
template <class Type>
typename MultiArray<Type,1>::Iterator& MultiArray<Type,1>::Iterator::operator++() {
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++i;
   if (i != a->Size()) {
      t = &((*a)[i]);
   } else {
      t = 0;
   }
   
   return *this;
}



template <>
template <class Type>
typename MultiArray<Type,1>::Iterator& MultiArray<Type,1>::Iterator::operator--() {
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {
      if (a->Size()) {
         --i;
         t = &((*a)[i]);
      }
      return *this;
   }
   if (i == 0) {
      *this = m->End();
      return *this;
   }
   --i;
   t = &((*a)[i]);
   return *this;
}



template <>
template <class Type>
typename MultiArray<Type , 1>::Iterator MultiArray<Type , 1>::Iterator::operator++(int) {
   Iterator iter = *this;
   ++(*this);
   return iter;

/** This doesn't return the same iterator as was passed to it
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++i;
   if (i != a->Size()) {
      t = &((*a)[i]);
   } else {
      t = 0;
   }
   
   return *this;
*/
}



template <>
template <class Type>
typename MultiArray<Type , 1>::Iterator MultiArray<Type , 1>::Iterator::operator--(int) {
   Iterator iter = *this;
   --(*this);
   return iter;
/** This doesn't return the same iterator as was passed to it
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {
      if (a->Size()) {
         --i;
         t = &((*a)[i]);
      }
      return *this;
   }
   if (i == 0) {
      *this = m->End();
      return *this;
   }
   --i;
   t = &((*a)[i]);
   return *this;
*/
}



template <>
template <class Type>
Type& MultiArray<Type,1>::Iterator::operator*() {
   return *t;
}



/// -----------------------    MultiArray<Type,1>::ConstIterator     -------------------------------------



template <>
template <class Type>
MultiArray<Type,1>::ConstIterator::ConstIterator() :
      m(0),
      a(0),
      t(0),
      i(0)
{}



template <>
template <class Type>
MultiArray<Type,1>::ConstIterator::ConstIterator(const MultiArray<Type , 1>* multiarray , unsigned int index) :
      m(0),
      a(0),
      t(0),
      i(0)
{
   if (!multiarray) {return;}
   m = multiarray;
   a = &(m->array);
   if (index >= a->Size()) {
      i = a->Size();
      return;
   }
   i = index;
   t = &((*a)[i]);
}



template <>
template <class Type>
MultiArray<Type,1>::ConstIterator::ConstIterator(const MultiArray<Type,1>::Iterator& iter) :
      m(iter.m),
      a(iter.a),
      t(iter.t),
      i(iter.i)
{}



template <>
template <class Type>
typename MultiArray<Type,1>::ConstIterator& MultiArray<Type,1>::ConstIterator::operator=(const MultiArray<Type,1>::Iterator& iter) {
   m = iter.m;
   a = iter.a;
   t = iter.t;
   i = iter.i;
   return *this;
}



template <>
template <class Type>
bool MultiArray<Type,1>::ConstIterator::operator==(const MultiArray<Type,1>::ConstIterator& iter) {
   return ((m == iter.m) &&
           (a == iter.a) &&
           (t == iter.t) &&
           (i == iter.i));
}



template <>
template <class Type>
bool MultiArray<Type,1>::ConstIterator::operator==(const MultiArray<Type,1>::Iterator& iter) {
   return ((m == iter.m) &&
           (a == iter.a) &&
           (t == iter.t) &&
           (i == iter.i));
}



template <>
template <class Type>
typename MultiArray<Type,1>::ConstIterator& MultiArray<Type,1>::ConstIterator::operator++() {
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++i;
   if (i != a->Size()) {
      t = &((*a)[i]);
   } else {
      t = 0;
   }
   
   return *this;
}



template <>
template <class Type>
typename MultiArray<Type,1>::ConstIterator& MultiArray<Type,1>::ConstIterator::operator--() {
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {
      if (a->Size()) {
         --i;
         t = &((*a)[i]);
      }
      return *this;
   }
   if (i == 0) {
      *this = m->End();
      return *this;
   }
   --i;
   t = &((*a)[i]);
   return *this;
}



template <>
template <class Type>
typename MultiArray<Type , 1>::ConstIterator MultiArray<Type , 1>::ConstIterator::operator++(int) {
   ConstIterator iter = *this;
   ++(*this);
   return iter;
/** This doesn't return the iterator that was passed to it
   if (!m) {return *this;}
   
   if (*this == m->End()) {
      *this = m->Begin();
      return *this;
   }
   
   ++i;
   if (i != a->Size()) {
      t = &((*a)[i]);
   } else {
      t = 0;
   }
   
   return *this;
*/
}



template <>
template <class Type>
typename MultiArray<Type , 1>::ConstIterator MultiArray<Type , 1>::ConstIterator::operator--(int) {
   ConstIterator iter = *this;
   --(*this);
   return iter;
/** This doesn't return the same iterator that was passed to it
   if (!m) {return *this;}
   if (*this == m->Begin()) {
      *this = m->End();
      return *this;
   }
   if (*this == m->End()) {
      if (a->Size()) {
         --i;
         t = &((*a)[i]);
      }
      return *this;
   }
   if (i == 0) {
      *this = m->End();
      return *this;
   }
   --i;
   t = &((*a)[i]);
   return *this;
*/
}



template <>
template <class Type>
const Type& MultiArray<Type,1>::ConstIterator::operator*() {
   return *t;
}




