// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME FastFrames_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "FastFrames/MainFrame.h"
#include "FastFrames/MainFrame.h"
#include "FastFrames/DefineHelpersDict.h"

// Header files passed via #pragma extra_include
#include "FastFrames/MainFrame.h"
#include "FastFrames/DefineHelpersDict.h"

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_MainFrame(void *p = nullptr);
   static void *newArray_MainFrame(Long_t size, void *p);
   static void delete_MainFrame(void *p);
   static void deleteArray_MainFrame(void *p);
   static void destruct_MainFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MainFrame*)
   {
      ::MainFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MainFrame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("MainFrame", ::MainFrame::Class_Version(), "FastFrames/MainFrame.h", 31,
                  typeid(::MainFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MainFrame::Dictionary, isa_proxy, 4,
                  sizeof(::MainFrame) );
      instance.SetNew(&new_MainFrame);
      instance.SetNewArray(&newArray_MainFrame);
      instance.SetDelete(&delete_MainFrame);
      instance.SetDeleteArray(&deleteArray_MainFrame);
      instance.SetDestructor(&destruct_MainFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MainFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::MainFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MainFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *ROOT6_FastFramesHelperAutoloadHook_Dictionary();
   static void ROOT6_FastFramesHelperAutoloadHook_TClassManip(TClass*);
   static void *new_ROOT6_FastFramesHelperAutoloadHook(void *p = nullptr);
   static void *newArray_ROOT6_FastFramesHelperAutoloadHook(Long_t size, void *p);
   static void delete_ROOT6_FastFramesHelperAutoloadHook(void *p);
   static void deleteArray_ROOT6_FastFramesHelperAutoloadHook(void *p);
   static void destruct_ROOT6_FastFramesHelperAutoloadHook(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ROOT6_FastFramesHelperAutoloadHook*)
   {
      ::ROOT6_FastFramesHelperAutoloadHook *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ROOT6_FastFramesHelperAutoloadHook));
      static ::ROOT::TGenericClassInfo 
         instance("ROOT6_FastFramesHelperAutoloadHook", "FastFrames/DefineHelpersDict.h", 15,
                  typeid(::ROOT6_FastFramesHelperAutoloadHook), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ROOT6_FastFramesHelperAutoloadHook_Dictionary, isa_proxy, 4,
                  sizeof(::ROOT6_FastFramesHelperAutoloadHook) );
      instance.SetNew(&new_ROOT6_FastFramesHelperAutoloadHook);
      instance.SetNewArray(&newArray_ROOT6_FastFramesHelperAutoloadHook);
      instance.SetDelete(&delete_ROOT6_FastFramesHelperAutoloadHook);
      instance.SetDeleteArray(&deleteArray_ROOT6_FastFramesHelperAutoloadHook);
      instance.SetDestructor(&destruct_ROOT6_FastFramesHelperAutoloadHook);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ROOT6_FastFramesHelperAutoloadHook*)
   {
      return GenerateInitInstanceLocal(static_cast<::ROOT6_FastFramesHelperAutoloadHook*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ROOT6_FastFramesHelperAutoloadHook*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ROOT6_FastFramesHelperAutoloadHook_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ROOT6_FastFramesHelperAutoloadHook*>(nullptr))->GetClass();
      ROOT6_FastFramesHelperAutoloadHook_TClassManip(theClass);
   return theClass;
   }

   static void ROOT6_FastFramesHelperAutoloadHook_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MainFrame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MainFrame::Class_Name()
{
   return "MainFrame";
}

//______________________________________________________________________________
const char *MainFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MainFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MainFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MainFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MainFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class MainFrame.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MainFrame::Class(),this);
   } else {
      R__b.WriteClassBuffer(MainFrame::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MainFrame(void *p) {
      return  p ? new(p) ::MainFrame : new ::MainFrame;
   }
   static void *newArray_MainFrame(Long_t nElements, void *p) {
      return p ? new(p) ::MainFrame[nElements] : new ::MainFrame[nElements];
   }
   // Wrapper around operator delete
   static void delete_MainFrame(void *p) {
      delete (static_cast<::MainFrame*>(p));
   }
   static void deleteArray_MainFrame(void *p) {
      delete [] (static_cast<::MainFrame*>(p));
   }
   static void destruct_MainFrame(void *p) {
      typedef ::MainFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::MainFrame

namespace ROOT {
   // Wrappers around operator new
   static void *new_ROOT6_FastFramesHelperAutoloadHook(void *p) {
      return  p ? new(p) ::ROOT6_FastFramesHelperAutoloadHook : new ::ROOT6_FastFramesHelperAutoloadHook;
   }
   static void *newArray_ROOT6_FastFramesHelperAutoloadHook(Long_t nElements, void *p) {
      return p ? new(p) ::ROOT6_FastFramesHelperAutoloadHook[nElements] : new ::ROOT6_FastFramesHelperAutoloadHook[nElements];
   }
   // Wrapper around operator delete
   static void delete_ROOT6_FastFramesHelperAutoloadHook(void *p) {
      delete (static_cast<::ROOT6_FastFramesHelperAutoloadHook*>(p));
   }
   static void deleteArray_ROOT6_FastFramesHelperAutoloadHook(void *p) {
      delete [] (static_cast<::ROOT6_FastFramesHelperAutoloadHook*>(p));
   }
   static void destruct_ROOT6_FastFramesHelperAutoloadHook(void *p) {
      typedef ::ROOT6_FastFramesHelperAutoloadHook current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ROOT6_FastFramesHelperAutoloadHook

namespace ROOT {
   static TClass *setlEpairlEunsignedsPintcOULong64_tgRsPgR_Dictionary();
   static void setlEpairlEunsignedsPintcOULong64_tgRsPgR_TClassManip(TClass*);
   static void *new_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p = nullptr);
   static void *newArray_setlEpairlEunsignedsPintcOULong64_tgRsPgR(Long_t size, void *p);
   static void delete_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p);
   static void deleteArray_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p);
   static void destruct_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<pair<unsigned int,ULong64_t> >*)
   {
      set<pair<unsigned int,ULong64_t> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<pair<unsigned int,ULong64_t> >));
      static ::ROOT::TGenericClassInfo 
         instance("set<pair<unsigned int,ULong64_t> >", -2, "set", 94,
                  typeid(set<pair<unsigned int,ULong64_t> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEpairlEunsignedsPintcOULong64_tgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(set<pair<unsigned int,ULong64_t> >) );
      instance.SetNew(&new_setlEpairlEunsignedsPintcOULong64_tgRsPgR);
      instance.SetNewArray(&newArray_setlEpairlEunsignedsPintcOULong64_tgRsPgR);
      instance.SetDelete(&delete_setlEpairlEunsignedsPintcOULong64_tgRsPgR);
      instance.SetDeleteArray(&deleteArray_setlEpairlEunsignedsPintcOULong64_tgRsPgR);
      instance.SetDestructor(&destruct_setlEpairlEunsignedsPintcOULong64_tgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<pair<unsigned int,ULong64_t> > >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("set<pair<unsigned int,ULong64_t> >","std::set<std::pair<unsigned int, unsigned long long>, std::less<std::pair<unsigned int, unsigned long long> >, std::allocator<std::pair<unsigned int, unsigned long long> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const set<pair<unsigned int,ULong64_t> >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEpairlEunsignedsPintcOULong64_tgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const set<pair<unsigned int,ULong64_t> >*>(nullptr))->GetClass();
      setlEpairlEunsignedsPintcOULong64_tgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEpairlEunsignedsPintcOULong64_tgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) set<pair<unsigned int,ULong64_t> > : new set<pair<unsigned int,ULong64_t> >;
   }
   static void *newArray_setlEpairlEunsignedsPintcOULong64_tgRsPgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) set<pair<unsigned int,ULong64_t> >[nElements] : new set<pair<unsigned int,ULong64_t> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p) {
      delete (static_cast<set<pair<unsigned int,ULong64_t> >*>(p));
   }
   static void deleteArray_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p) {
      delete [] (static_cast<set<pair<unsigned int,ULong64_t> >*>(p));
   }
   static void destruct_setlEpairlEunsignedsPintcOULong64_tgRsPgR(void *p) {
      typedef set<pair<unsigned int,ULong64_t> > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class set<pair<unsigned int,ULong64_t> >

namespace ROOT {
   static TClass *maplEstringcOstringgR_Dictionary();
   static void maplEstringcOstringgR_TClassManip(TClass*);
   static void *new_maplEstringcOstringgR(void *p = nullptr);
   static void *newArray_maplEstringcOstringgR(Long_t size, void *p);
   static void delete_maplEstringcOstringgR(void *p);
   static void deleteArray_maplEstringcOstringgR(void *p);
   static void destruct_maplEstringcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,string>*)
   {
      map<string,string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,string>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,string>", -2, "map", 100,
                  typeid(map<string,string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOstringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,string>) );
      instance.SetNew(&new_maplEstringcOstringgR);
      instance.SetNewArray(&newArray_maplEstringcOstringgR);
      instance.SetDelete(&delete_maplEstringcOstringgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOstringgR);
      instance.SetDestructor(&destruct_maplEstringcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,string> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<string,string>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<string,string>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<string,string>*>(nullptr))->GetClass();
      maplEstringcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOstringgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<string,string> : new map<string,string>;
   }
   static void *newArray_maplEstringcOstringgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<string,string>[nElements] : new map<string,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOstringgR(void *p) {
      delete (static_cast<map<string,string>*>(p));
   }
   static void deleteArray_maplEstringcOstringgR(void *p) {
      delete [] (static_cast<map<string,string>*>(p));
   }
   static void destruct_maplEstringcOstringgR(void *p) {
      typedef map<string,string> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<string,string>

namespace ROOT {
   static TClass *maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR_Dictionary();
   static void maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p);
   static void deleteArray_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p);
   static void destruct_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,set<pair<unsigned int,ULong64_t> > >*)
   {
      map<string,set<pair<unsigned int,ULong64_t> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,set<pair<unsigned int,ULong64_t> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,set<pair<unsigned int,ULong64_t> > >", -2, "map", 100,
                  typeid(map<string,set<pair<unsigned int,ULong64_t> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,set<pair<unsigned int,ULong64_t> > >) );
      instance.SetNew(&new_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR);
      instance.SetDelete(&delete_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,set<pair<unsigned int,ULong64_t> > > >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<string,set<pair<unsigned int,ULong64_t> > >","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::set<std::pair<unsigned int, unsigned long long>, std::less<std::pair<unsigned int, unsigned long long> >, std::allocator<std::pair<unsigned int, unsigned long long> > >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::set<std::pair<unsigned int, unsigned long long>, std::less<std::pair<unsigned int, unsigned long long> >, std::allocator<std::pair<unsigned int, unsigned long long> > > > > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<string,set<pair<unsigned int,ULong64_t> > >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<string,set<pair<unsigned int,ULong64_t> > >*>(nullptr))->GetClass();
      maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<string,set<pair<unsigned int,ULong64_t> > > : new map<string,set<pair<unsigned int,ULong64_t> > >;
   }
   static void *newArray_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<string,set<pair<unsigned int,ULong64_t> > >[nElements] : new map<string,set<pair<unsigned int,ULong64_t> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p) {
      delete (static_cast<map<string,set<pair<unsigned int,ULong64_t> > >*>(p));
   }
   static void deleteArray_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p) {
      delete [] (static_cast<map<string,set<pair<unsigned int,ULong64_t> > >*>(p));
   }
   static void destruct_maplEstringcOsetlEpairlEunsignedsPintcOULong64_tgRsPgRsPgR(void *p) {
      typedef map<string,set<pair<unsigned int,ULong64_t> > > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<string,set<pair<unsigned int,ULong64_t> > >

namespace ROOT {
   static TClass *maplEstringcOmaplEstringcOstringgRsPgR_Dictionary();
   static void maplEstringcOmaplEstringcOstringgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOmaplEstringcOstringgRsPgR(void *p = nullptr);
   static void *newArray_maplEstringcOmaplEstringcOstringgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOmaplEstringcOstringgRsPgR(void *p);
   static void deleteArray_maplEstringcOmaplEstringcOstringgRsPgR(void *p);
   static void destruct_maplEstringcOmaplEstringcOstringgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,map<string,string> >*)
   {
      map<string,map<string,string> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,map<string,string> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,map<string,string> >", -2, "map", 100,
                  typeid(map<string,map<string,string> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOmaplEstringcOstringgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,map<string,string> >) );
      instance.SetNew(&new_maplEstringcOmaplEstringcOstringgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOmaplEstringcOstringgRsPgR);
      instance.SetDelete(&delete_maplEstringcOmaplEstringcOstringgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOmaplEstringcOstringgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOmaplEstringcOstringgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,map<string,string> > >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<string,map<string,string> >","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > > > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<string,map<string,string> >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOmaplEstringcOstringgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<string,map<string,string> >*>(nullptr))->GetClass();
      maplEstringcOmaplEstringcOstringgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOmaplEstringcOstringgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOmaplEstringcOstringgRsPgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<string,map<string,string> > : new map<string,map<string,string> >;
   }
   static void *newArray_maplEstringcOmaplEstringcOstringgRsPgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<string,map<string,string> >[nElements] : new map<string,map<string,string> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOmaplEstringcOstringgRsPgR(void *p) {
      delete (static_cast<map<string,map<string,string> >*>(p));
   }
   static void deleteArray_maplEstringcOmaplEstringcOstringgRsPgR(void *p) {
      delete [] (static_cast<map<string,map<string,string> >*>(p));
   }
   static void destruct_maplEstringcOmaplEstringcOstringgRsPgR(void *p) {
      typedef map<string,map<string,string> > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<string,map<string,string> >

namespace {
  void TriggerDictionaryInitialization_libFastFrames_Impl() {
    static const char* headers[] = {
"FastFrames/MainFrame.h",
nullptr
    };
    static const char* includePaths[] = {
"/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames",
"/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/include",
"/afs/cern.ch/user/n/nsangwen/FastFrames/FastFrames",
"/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/0.4/StatAnalysis/0.4.2/InstallArea/x86_64-el9-gcc13-opt/include/",
"/afs/cern.ch/user/n/nsangwen/FastFrames/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libFastFrames dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$FastFrames/MainFrame.h")))  MainFrame;
struct __attribute__((annotate("$clingAutoload$FastFrames/DefineHelpersDict.h")))  ROOT6_FastFramesHelperAutoloadHook;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libFastFrames dictionary payload"

#ifndef BOOST_BIND_GLOBAL_PLACEHOLDERS
  #define BOOST_BIND_GLOBAL_PLACEHOLDERS 1
#endif
#ifndef BOOST_ALLOW_DEPRECATED_HEADERS
  #define BOOST_ALLOW_DEPRECATED_HEADERS 1
#endif
#ifndef ONNXRUNTIME_AVAILABLE
  #define ONNXRUNTIME_AVAILABLE 1
#endif
#ifndef BOOST_BIND_GLOBAL_PLACEHOLDERS
  #define BOOST_BIND_GLOBAL_PLACEHOLDERS 1
#endif
#ifndef BOOST_ALLOW_DEPRECATED_HEADERS
  #define BOOST_ALLOW_DEPRECATED_HEADERS 1
#endif
#ifndef ONNXRUNTIME_AVAILABLE
  #define ONNXRUNTIME_AVAILABLE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "FastFrames/MainFrame.h"
/**
 * @file LinkDef.h
 * @brief File needed for ROOT dictionaries creation
 *
 */

#include "FastFrames/MainFrame.h"
#include "FastFrames/DefineHelpersDict.h"

#ifdef __CINT__

#pragma extra_include "FastFrames/MainFrame.h";
#pragma extra_include "FastFrames/DefineHelpersDict.h";
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ class MainFrame+;
#pragma link C++ class ROOT6_FastFramesHelperAutoloadHook+;

#endif

// Extra includes
#include "FastFrames/MainFrame.h"
#include "FastFrames/DefineHelpersDict.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"MainFrame", payloadCode, "@",
"ROOT6_FastFramesHelperAutoloadHook", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libFastFrames",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libFastFrames_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libFastFrames_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libFastFrames() {
  TriggerDictionaryInitialization_libFastFrames_Impl();
}
