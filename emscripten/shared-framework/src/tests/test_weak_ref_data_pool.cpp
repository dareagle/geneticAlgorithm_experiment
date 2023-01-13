
#include "framework/system/containers/weak_ref_data_pool.hpp"

#include "framework/system/TraceLogger.hpp"

#include <cassert>

#include "common.hpp"

void test_weak_ref_data_pool() {
  D_MYLOG("test_weak_ref_data_pool()");

  {
    weak_ref_data_pool<common::Test, common::Test, 10, true> myPool;

    // myPool.initialiseSize(5);
    // myPool.pre_allocate(10);

    // std::cout << "myPool.size()=" << myPool.size() << std::endl;

    auto ref1A = myPool.acquire(555);
    auto ref1B = ref1A;
    auto ref1C = ref1B;

    auto ref1D_moved = ref1C;
    auto ref1E = std::move(ref1D_moved);

    assert(myPool.size() == 1);
    assert(myPool.get_index(ref1A) == 0);
    assert(ref1A.is_active() == true);
    assert(ref1B.is_active() == true);
    assert(ref1C.is_active() == true);
    assert(ref1D_moved.is_active() == false);
    assert(ref1E.is_active() == true);
    assert(ref1A == true);
    assert(ref1B == true);
    assert(ref1C == true);
    assert(ref1D_moved == false);
    assert(ref1E == true);
    assert(ref1A.get() == myPool.get(0).get());
    assert(ref1D_moved.get() == nullptr);
    assert(ref1A.get() == ref1B.get());
    assert(ref1A.get() == ref1C.get());
    assert(ref1A.get() == ref1E.get());

    // std::cout << "myPool.size()=" << myPool.size() << std::endl;

    auto ref2A = myPool.acquire(777777);
    assert(myPool.size() == 2);
    assert(myPool.get_index(ref2A) == 1);
    assert(ref2A.get() == myPool.get(1).get());
    assert(ref2A.is_active() == true);
    assert(ref2A->value == 777777);

    // std::cout << "myPool.size()=" << myPool.size() << std::endl;

    assert(ref1A->value == 555);
    assert(ref1B->value == 555);
    assert(ref1C->value == 555);
    assert(ref1E->value == 555);

    ref1A->value = 666;

    assert(ref1A.is_active() == true);
    assert(ref1B.is_active() == true);
    assert(ref1C.is_active() == true);
    assert(ref1D_moved.is_active() == false);
    assert(ref1E.is_active() == true);

    assert(ref1A->value == 666);
    assert(ref1B->value == 666);
    assert(ref1C->value == 666);
    assert(ref1E->value == 666);

    // std::cout << "ref1A=" << ref1A->value << ", " << ref1A << ", " <<
    // ref1A.is_active() << std::endl; std::cout << "ref1B=" << ref1B->value <<
    // ", " << ref1B << ", " << ref1B.is_active() << std::endl; std::cout <<
    // "ref1C=" << ref1C->value << ", " << ref1C << ", " << ref1C.is_active() <<
    // std::endl; std::cout << "ref1E=" << ref1E->value << ", " << ref1E << ", "
    // << ref1E.is_active() << std::endl;

    // std::cout << "RELEASE REF1A" << std::endl;

    assert(ref1A.is_active() == true);

    myPool.release(ref1A);

    assert(ref1A.is_active() == false);
    assert(ref1B.is_active() == false);
    assert(ref1C.is_active() == false);
    assert(ref1D_moved.is_active() == false);
    assert(ref1E.is_active() == false);

    assert(ref2A.is_active() == true);

    assert(myPool.size() == 1);
    assert(myPool.get_index(ref2A) == 0);
    assert(ref2A.get() == myPool.get(0).get());
    assert(ref2A.is_active() == true);
    assert(ref2A->value == 777777);

    // std::cout << "ref1A=" << ref1A << std::endl;
    // std::cout << "ref1B=" << ref1B << std::endl;
    // std::cout << "ref1C=" << ref1C << std::endl;
    // std::cout << "ref1E=" << ref1E << std::endl;
    // std::cout << "ref2A=" << ref2A->value << ", " << ref2A << std::endl;

    myPool.clear();

    assert(myPool.size() == 0);
    assert(ref1A.is_active() == false);
    assert(ref1B.is_active() == false);
    assert(ref1C.is_active() == false);
    assert(ref1E.is_active() == false);
    assert(ref1A.get() == nullptr);
    assert(ref1B.get() == nullptr);
    assert(ref1C.get() == nullptr);
    assert(ref1E.get() == nullptr);

    assert(ref2A == false);
    assert(ref2A.get() == nullptr);

    {
      common::reset();

      for (int ii = 0; ii < 20; ++ii)
        myPool.acquire(ii);

      assert(myPool.size() == 10);

      myPool.filter([](common::Test& item) {
        // std::cout << " (" << item.value << " % 2)=" << ((item.value % 2) ==
        // 0) << std::endl;

        return ((item.value % 2) == 0); // keep only the even values
      });

      // std::cout << "myPool.size()=" << myPool.size() << std::endl;

      assert(myPool.size() == 5);
      assert(myPool.get(0).get()->value == 0);
      assert(myPool.get(1).get()->value == 8);
      assert(myPool.get(2).get()->value == 2);
      assert(myPool.get(3).get()->value == 6);
      assert(myPool.get(4).get()->value == 4);

      // std::cout << "myPool.size()=" << myPool.size() << std::endl;

      // print();

      assert(common::getTotalCtor() == 10);
      assert(common::getTotalCopyCtor() == 0);
      // assert(common::getTotalMoveCtor() == 4);
      assert(common::getTotalDtor() == 5);
    }

    // std::cout << "myPool.size()=" << myPool.size() << std::endl;

    // std::cout << "ref1A=" << ref1A.get() << ", " << ref1A << std::endl;
    // std::cout << "ref1B=" << ref1B.get() << ", " << ref1B << std::endl;
    // std::cout << "ref1C=" << ref1C.get() << ", " << ref1C << std::endl;
    // std::cout << "ref1E=" << ref1E.get() << ", " << ref1E << std::endl;
    // std::cout << "ref2A=" << ref2A.get() << ", " << ref2A << std::endl;
  }

  {
    weak_ref_data_pool<common::Test, common::Test, 10, false> myPool;

    for (int ii = 0; ii < 20; ++ii)
      myPool.acquire(ii);

    assert(myPool.size() == 20); // reallocate was allowed
  }

  {
    weak_ref_data_pool<common::Test2, common::Test2, 10, false> myPool;

    auto ref = myPool.acquire();
    ref->value = 111;

    common::Test2 lol;
    lol.value = 222;

    *ref = std::move(lol);

    D_MYLOG("lol.value " << lol.value);
    D_MYLOG("ref->value " << ref->value);

    assert(ref->value == 222);
    assert(lol.value == 111);
  }

  {
    weak_ref_data_pool<common::Test, common::Test, 10, false> myPool1;
    weak_ref_data_pool<common::Test, common::Test, 10, false> myPool2;

    for (int ii = 0; ii < 20; ++ii)
      myPool1.acquire(ii);

    auto ref10 = myPool1.get(10);
    assert(ref10->value == 10);

    assert(myPool1.size() == 20);
    assert(myPool2.size() == 0);

    myPool2 = std::move(myPool1);

    assert(ref10->value == 10);

    assert(myPool1.size() == 0);
    assert(myPool2.size() == 20);
  }

  {
    using LocalPool = weak_ref_data_pool<common::Test, common::Test, 10, false>;
    using LocalRef = LocalPool::weak_ref;

    LocalRef ref1 = LocalPool::weak_ref::make_invalid();
    LocalRef ref2 = LocalPool::weak_ref::make_invalid();
    LocalRef ref3 = LocalPool::weak_ref::make_invalid();

    {
      LocalPool myPool;

      assert(myPool.get_ref_count(0) == 0);
      assert(myPool.get_ref_count(1) == 0);
      assert(myPool.get_ref_count(2) == 0);

      // LocalRef ref1 = myPool.acquire(666);
      // LocalRef ref2 = myPool.acquire(666);
      // LocalRef ref3 = myPool.acquire(666);
      ref1 = myPool.acquire(666);
      ref2 = myPool.acquire(666);
      ref3 = myPool.acquire(666);

      assert(ref1);
      assert(ref2);
      assert(ref3);
      assert(myPool.get_ref_count(0) == 1);
      assert(myPool.get_ref_count(1) == 1);
      assert(myPool.get_ref_count(2) == 1);

      ref2.invalidate();

      assert(ref1);
      assert(!ref2);
      assert(ref3);
      assert(myPool.get_ref_count(0) == 1);
      assert(myPool.get_ref_count(1) == 0);
      assert(myPool.get_ref_count(2) == 1);

      {
        LocalRef tmpRef = myPool.acquire(666);

        assert(ref1);
        assert(!ref2);
        assert(ref3);
        assert(tmpRef);
        assert(myPool.get_ref_count(0) == 1);
        assert(myPool.get_ref_count(1) == 0);
        assert(myPool.get_ref_count(2) == 1);
        assert(myPool.get_ref_count(3) == 1);
      }

      assert(ref1);
      assert(!ref2);
      assert(ref3);
      assert(myPool.get_ref_count(0) == 1);
      assert(myPool.get_ref_count(1) == 0);
      assert(myPool.get_ref_count(2) == 1);
      assert(myPool.get_ref_count(3) == 0);

      myPool.clear();

      assert(!ref1);
      assert(!ref2);
      assert(!ref3);
      assert(myPool.get_ref_count(0) == 0);
      assert(myPool.get_ref_count(1) == 0);
      assert(myPool.get_ref_count(2) == 0);

      ref1 = myPool.acquire(666);
      ref2 = myPool.acquire(666);
      ref3 = myPool.acquire(666);

      assert(ref1);
      assert(ref2);
      assert(ref3);
      assert(myPool.get_ref_count(0) == 1);
      assert(myPool.get_ref_count(1) == 1);
      assert(myPool.get_ref_count(2) == 1);
    }

    assert(!ref1);
    assert(!ref2);
    assert(!ref3);

    // assert(myPool.size() == 20); // reallocate was allowed
  }

  {

    using LocalContainer = dynamic_heap_array<common::Test>;
    using ContainersPool =
      weak_ref_data_pool<LocalContainer, LocalContainer, 16, false>;
    using ContainerPoolRef = ContainersPool::weak_ref;

    using RefsPool =
      weak_ref_data_pool<ContainerPoolRef, ContainerPoolRef, 32, false>;
    using RefPoolRef = RefsPool::weak_ref;

    // LocalRef ref1 = LocalPool::weak_ref::make_invalid();
    // LocalRef ref2 = LocalPool::weak_ref::make_invalid();
    // LocalRef ref3 = LocalPool::weak_ref::make_invalid();

    {
      ContainersPool myContainersPool;
      RefsPool myRefsPool;

      {
        ContainerPoolRef mainRef = myContainersPool.acquire();
        mainRef->push_back(common::Test(666));
        mainRef->push_back(common::Test(777));
        mainRef->push_back(common::Test(888));

        RefPoolRef subRef = myRefsPool.acquire();
        (*subRef) = std::move(mainRef);

        ContainerPoolRef mainRef2 = myContainersPool.get(0);

        assert((*subRef)->at(0).value == mainRef2->at(0).value);
      }
    }
  }

  {
    weak_ref_data_pool<common::Test, common::Test, 10, true> myPool;

    // myPool.initialiseSize(5);
    // myPool.pre_allocate(10);

    // std::cout << "myPool.size()=" << myPool.size() << std::endl;

    auto ref1A = myPool.acquire(555);
    auto ref1B = ref1A;
    auto ref1C = ref1B;

    auto ref1D_moved = ref1C;
    auto ref1E = std::move(ref1D_moved);

    assert(myPool.size() == 1);
    assert(myPool.get_index(ref1A) == 0);
    assert(ref1A.is_active() == true);
    assert(ref1B.is_active() == true);
    assert(ref1C.is_active() == true);
    assert(ref1D_moved.is_active() == false);
    assert(ref1E.is_active() == true);
    assert(ref1A == true);
    assert(ref1B == true);
    assert(ref1C == true);
    assert(ref1D_moved == false);
    assert(ref1E == true);
    assert(ref1A.get() == myPool.get(0).get());
    assert(ref1D_moved.get() == nullptr);
    assert(ref1A.get() == ref1B.get());
    assert(ref1A.get() == ref1C.get());
    assert(ref1A.get() == ref1E.get());

    myPool.clear();

    assert(myPool.size() == 0);
    assert(myPool.get_index(ref1A) == -1);
    assert(ref1A.is_active() == false);
    assert(ref1B.is_active() == false);
    assert(ref1C.is_active() == false);
    assert(ref1D_moved.is_active() == false);
    assert(ref1E.is_active() == false);
    assert(ref1A == false);
    assert(ref1B == false);
    assert(ref1C == false);
    assert(ref1D_moved == false);
    assert(ref1E == false);
    assert(ref1A.get() == nullptr);
    assert(ref1B.get() == nullptr);
    assert(ref1C.get() == nullptr);
    assert(ref1D_moved.get() == nullptr);
    assert(ref1E.get() == nullptr);
  }

  {
    using Pool = weak_ref_data_pool<common::Test, common::Test, 10, true>;
    using PoolRef = Pool::weak_ref;

    struct MyAgg {

      struct MySubAgg1 {
        PoolRef ref1;
        PoolRef ref2;
      } mySubAgg1;

      struct MySubAgg2 {
        PoolRef ref1;
        PoolRef ref2;
        Pool myPool;
        PoolRef ref3;
        PoolRef ref4;
      } mySubAgg2;

      struct MySubAgg3 {
        PoolRef ref1;
        PoolRef ref2;
      } mySubAgg3;
    };

    {
      PoolRef ref1;
      PoolRef ref2;

      {
        MyAgg localAgg;

        // localAgg.ref1 = localAgg.myPool.acquire();
        // localAgg.ref2 = localAgg.myPool.acquire();
        ref1 = localAgg.mySubAgg2.myPool.acquire();
        ref2 = localAgg.mySubAgg2.myPool.acquire();
        localAgg.mySubAgg1.ref1 = localAgg.mySubAgg2.myPool.acquire();
        localAgg.mySubAgg1.ref2 = localAgg.mySubAgg2.myPool.acquire();
        localAgg.mySubAgg3.ref1 = localAgg.mySubAgg2.myPool.acquire();
        localAgg.mySubAgg3.ref2 = localAgg.mySubAgg2.myPool.acquire();

        // localAgg.ref3 = localAgg.mySubAgg2.myPool.acquire();

        // localAgg.mySubAgg2.myPool.clear();

        // ref2 = localAgg.mySubAgg2.myPool.acquire();
        // localAgg.ref4 = localAgg.mySubAgg2.myPool.acquire();
      }
    }
  }

  {
    common::reset();
    assert(common::getTotalCtor() == 0);
    assert(common::getTotalCopyCtor() == 0);
    assert(common::getTotalMoveCtor() == 0);
    assert(common::getTotalDtor() == 0);

    weak_ref_data_pool<common::Test, common::Test, 10, true> myPool;

    auto ref1 = myPool.acquire(111);
    auto ref2 = myPool.acquire(222);
    auto ref3 = myPool.acquire(333);
    auto ref4 = myPool.acquire(444);
    auto ref5 = myPool.acquire(555);

    assert(myPool.size() == 5);
    assert(ref1.is_active() == true);
    assert(ref2.is_active() == true);
    assert(ref3.is_active() == true);
    assert(ref4.is_active() == true);
    assert(ref5.is_active() == true);
    assert(ref1->value == 111);
    assert(ref2->value == 222);
    assert(ref3->value == 333);
    assert(ref4->value == 444);
    assert(ref5->value == 555);

    // common::print();
    assert(common::getTotalCtor() == 5);
    assert(common::getTotalCopyCtor() == 0);
    assert(common::getTotalMoveCtor() == 0);
    assert(common::getTotalDtor() == 0);
    common::reset();

    myPool.release(ref3);

    assert(myPool.size() == 4);
    assert(ref1.is_active() == true);
    assert(ref2.is_active() == true);
    assert(ref3.is_active() == false);
    assert(ref4.is_active() == true);
    assert(ref5.is_active() == true);
    assert(ref1->value == 111);
    assert(ref2->value == 222);
    // assert(ref3->value == 333);
    assert(ref4->value == 444);
    assert(ref5->value == 555);

    // common::print();
    assert(common::getTotalCtor() == 0);
    assert(common::getTotalCopyCtor() == 0);
    // assert(common::getTotalMoveCtor() == 1);
    assert(common::getTotalDtor() == 1);
    common::reset();

    // assert(myPool.size() == 0);
    // assert(myPool.get_index(ref1A) == -1);
    // assert(ref1A.is_active() == false);
    // assert(ref1B.is_active() == false);
    // assert(ref1C.is_active() == false);
    // assert(ref1D_moved.is_active() == false);
    // assert(ref1E.is_active() == false);
    // assert(ref1A == false);
    // assert(ref1B == false);
    // assert(ref1C == false);
    // assert(ref1D_moved == false);
    // assert(ref1E == false);
    // assert(ref1A.get() == nullptr);
    // assert(ref1B.get() == nullptr);
    // assert(ref1C.get() == nullptr);
    // assert(ref1D_moved.get() == nullptr);
    // assert(ref1E.get() == nullptr);
  }

  D_MYLOG(" => DONE");
}
