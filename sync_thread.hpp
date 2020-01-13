#ifndef MULTITHREADS_SYNC_THREAD_HPP
#define MULTITHREADS_SYNC_THREAD_HPP
#include <type_traits>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <cstring>

using std::atomic;
using std::unique_lock;
using std::condition_variable;
using std::mutex;
using std::enable_if_t;
using std::is_pointer_v;
using std::remove_reference_t;

//TODO avoid clone object in put
//TODO optimize with memory order
//TODO think about improve method get
//TODO create exception handler
//TODO create error code integration
//TODO create check if return null ptr and other C-style function problem
//TODO check get function for UB and strange behavior


class object_sync{
private:

    atomic<void *> obj;
    mutex mut;
    condition_variable cv;

public:
    enum error_sync{
        ignore,
        signalize,
    };

    enum type_sync{
        copy_object,
        same_object,
        reference_object,
    };

    template<typename T, error_sync warning = error_sync::signalize/*, type_sync ts,
            enable_if_t<ts == copy_object, int> = 0*/>
    void put(T&& share_object){
        static_assert(!(warning == error_sync::signalize && std::is_pointer_v<T>));

        unique_lock lk(mut);
        void *tpm = reinterpret_cast<void *>(new remove_reference_t<T>(share_object));
        obj.store(tpm);
        cv.notify_one();
    }

    template<typename T, error_sync warning = error_sync::signalize, type_sync ts,
            enable_if_t<ts == copy_object, int> = 0>
    void put(T&& share_object){
        static_assert(!(warning == error_sync::signalize && std::is_pointer_v<T>));

        unique_lock lk(mut);
        void *tpm = reinterpret_cast<void *>(new remove_reference_t<T>(share_object));
        obj.store(tpm);
        cv.notify_one();
    }

    template<typename T, error_sync warning = error_sync::signalize, type_sync ts,
            enable_if_t<ts == same_object, int> = 0>
    void put(T&& share_object){
        static_assert(!(warning == error_sync::signalize && std::is_pointer_v<T>));

        unique_lock lk(mut);
        void *tpm = malloc(sizeof(remove_reference_t<T>));
        std::memcpy(tpm, &share_object, sizeof(remove_reference_t<T>));
        obj.store(tpm);
        cv.notify_one();
    }

    template<typename T, error_sync warning = error_sync::signalize, type_sync ts,
            enable_if_t<ts == reference_object, int> = 0>
    void put(T&& share_object){
        static_assert(!(warning == error_sync::signalize && std::is_pointer_v<T>));

        unique_lock lk(mut);
        obj.store(reinterpret_cast<void *>((&share_object)));
        cv.notify_one();
    }

    void *get(){
        unique_lock lk(mut);
        //if put notify will be before get wait
        if(!obj.load())
            cv.wait(lk);
        return obj.load();
    }

    template<typename T>
    T& get(){
        unique_lock lk(mut);
        if(!obj.load())
            cv.wait(lk);
        T& tmp = *(reinterpret_cast<T *>(obj.load()));
        return tmp;
    }
};



#endif //MULTITHREADS_SYNC_THREAD_HPP
