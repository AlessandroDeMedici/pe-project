#ifndef __CLOUDCOMPUTINGWORKLOADS_COHERENCECHECK_H_
#define __CLOUDCOMPUTINGWORKLOADS_COHERENCECHECK_H_

// Support class for coherence check that keeps track of the number of tasks in a node at any given time
class CoherenceCheck {
protected:
    unsigned long nTasks = 0;

public:
    CoherenceCheck() = default;
    virtual ~CoherenceCheck() = default;

    // Getter for nTask
    virtual unsigned long getNTasks() const {
        return nTasks;
    }

    // Setter for nTask
    virtual void setNTasks(unsigned long n) {
        nTasks = n;
    }
    
    // Increment nTask
    virtual void incrementNTasks() {
        nTasks++;
    }
};

#endif

