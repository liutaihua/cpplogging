namespace Crown
{
    template <typename TStream>
    void Printf(TStream& out, boost::format& fmt)
    {
            out << fmt;
    }

    template <typename TStream, typename THead, typename ... Tail>
    void Printf(TStream& out, boost::format& fmt, const THead& head, const Tail&... args)
    {
            Printf(out, fmt % head, args...);
    }


    template <typename TStream, typename ... Args>
    void Printf(TStream& out, const char* s, const Args&... args)
    {
            if (sizeof...(Args))
            {
                    boost::format fmt(s);
                    Printf(out, fmt, args...);
            }
            else
            {
                    out << s;
            }
    }

    enum ConstructorStrategy
    {
            WITH_DEFAULT_CONSTRUCTOR,
            WITHOUT_DEFAULT_CONSTRUCTOR
    };
}

template <typename T>
class _Singleton
{
private:
        _Singleton(const _Singleton<T>&) {}
        _Singleton& operator =(const _Singleton<T>&) {}

protected:
        // You SHOULD initialize the instance yourself.
        static T* m_pInstance;

        _Singleton() {}
};

template <typename T>
T* _Singleton<T>::m_pInstance = nullptr;

template <typename T, enum Crown::ConstructorStrategy>
class Singleton;

template <typename T>
class Singleton<T, Crown::WITH_DEFAULT_CONSTRUCTOR> : public _Singleton<T>
{
    struct Initializer
    {
        Initializer(T** ppInstance)
        {
            if (*ppInstance == nullptr)
                *ppInstance = new T;
        }
    };

public:
    static T& GetInstance()
    {
        static Initializer s_Instance(&_Singleton<T>::m_pInstance);
        return *_Singleton<T>::m_pInstance;
    }

    static T* GetInstancePtr()
    {
        static Initializer s_Instance(&_Singleton<T>::m_pInstance);
        return _Singleton<T>::m_pInstance;
    }
};

// 请自行初始化 m_pInstance 对象.
// WARNING: Initialize the instance yourself.
template <typename T>
class Singleton<T, Crown::WITHOUT_DEFAULT_CONSTRUCTOR> : public _Singleton<T>
{
public:
    static T& GetInstance()
    {
        return *_Singleton<T>::m_pInstance;
    }

    static T* GetInstancePtr()
    {
        return _Singleton<T>::m_pInstance;
    }
};
