#ifndef __CLOUDCOMPUTINGWORKLOADS_COHERENCECHECK_H_
#define __CLOUDCOMPUTINGWORKLOADS_COHERENCECHECK_H_

class CoherenceCheck {
protected:
    unsigned long nTasks = 0;

public:
    CoherenceCheck() = default;
    virtual ~CoherenceCheck() = default;

    // Getter per numeroTask
    virtual unsigned long getNTasks() const {
        return nTasks;
    }

    // Metodi per modificare numeroTask
    virtual void setNTasks(unsigned long n) {
        nTasks = n;
    }
    
    virtual void incrementNTasks() {
        nTasks++;
    }
};

#endif

