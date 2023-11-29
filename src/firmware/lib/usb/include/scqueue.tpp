
//----------------------------------------------------------------------------
//  Simple Circular Queue
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------


namespace simple_circular_queue {
    
    template <typename T, size_t allocation_size> 
    SCQueue<T, allocation_size>::SCQueue(void) {
#ifdef SCQ_RP2040_MUTEX
        mutex_init(&scq_mutex_);
#endif
    }


    template <typename T, size_t allocation_size> 
    bool SCQueue<T, allocation_size>::enqueue(T item) {
        bool enqueued = true;
        
        mutexLock();
        size_t new_last = (last_ + 1) % capacity_;
        
        // no overwriting, return false if queue is full
        if (!is_empty_ && first_ == new_last) {
            enqueued = false;
        }
        else {
            if(is_empty_) {
                queue_[last_] = item;
                is_empty_ = false;
            }
            else {
                last_ = new_last;
                queue_[last_] = item;
                is_empty_ = false;
            }
        }
        mutexUnlock();
        return enqueued;
    }
    
    template <typename T, size_t allocation_size> 
    T SCQueue<T, allocation_size>::dequeue(void) {
        // return NULL if no items
        T item = NULL;
        mutexLock();
        if (!is_empty_) {
            // last item to dequeue
            if (first_ == last_) {
                item = queue_[first_];
                is_empty_ = true;
            }
            else {
                size_t old_first = first_;
                first_ = (first_ + 1) % capacity_;
                item = queue_[old_first];
            }
        }
        mutexUnlock();
        return item;
    }
   
    template <typename T, size_t allocation_size> 
    T SCQueue<T, allocation_size>::peek(void) {
        T peek_value = NULL;
        mutexLock();
        if (!is_empty_) {
            peek_value = queue_[first_]; 
        }
        mutexUnlock();
        return peek_value;
    }

    template <typename T, size_t allocation_size> 
    bool SCQueue<T, allocation_size>::isEmpty(void) {
        mutexLock();
        volatile bool is_e = is_empty_; 
        mutexUnlock();
        return is_e;
    }
        
    template <typename T, size_t allocation_size> 
    void SCQueue<T, allocation_size>::mutexLock(void) {
#ifdef SCQ_RP2040_MUTEX        
        mutex_enter_blocking(&scq_mutex_);
#endif
    }

    template <typename T, size_t allocation_size> 
    void SCQueue<T, allocation_size>::mutexUnlock(void) {
#ifdef SCQ_RP2040_MUTEX        
        mutex_exit(&scq_mutex_);
#endif
    }
    template <typename T, size_t allocation_size> 
    void SCQueue<T, allocation_size>::log_values()
    {
        Logmsg.print("SCQ: head ");
        Logmsg.print(first_, fmtDEC);
        Logmsg.print(" tail ");
        Logmsg.print(last_, fmtDEC);
        Logmsg.print(" len ");
        Logmsg.println(capacity_, fmtDEC);
    }
}