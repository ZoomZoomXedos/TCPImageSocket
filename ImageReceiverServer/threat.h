#ifndef THREAT_H
#define THREAT_H
#include <QThread>
#include <QQueue>
#include <QList>
#include <QHash>

class threat
{
public:
    ~threat();

    enum thread_type
    {
        THREAD_SIZE,
        HANDLE_SIZE
    };

    static threat & get_class();

    QThread * get_thread();
    void init_thread_type(thread_type type = HANDLE_SIZE,unsigned int max = 1000);
    void remove_thread(QThread *);
    void clear();

protected:
    void init_thread_size();
    QThread * find_thread_size();
    QThread *find_handle_size();

private:
    threat();
    thread_type type;
    unsigned int size;
    QHash<QThread *, unsigned int> threadSize;
    bool initfist;
};

#endif // THREAT_H
