#ifndef __MEM_LOCK_H
#define __MEM_LOCK_H

class CMemLock
{
public:
    CMemLock();
    ~CMemLock();
public:
    //�����ڴ����
    void Lock();
    //�����ڴ����
    void Unlock();
private:
    HANDLE           m_hMutex;
};
#endif // !__MEM_LOCK_H
