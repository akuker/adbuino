
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

#pragma once

#include <stdlib.h>
#include <platform_logmsg.h>
#ifdef SCQ_RP2040_MUTEX
#include "pico/mutex.h"
#endif

namespace simple_circular_queue
{
    template <typename T, size_t allocation_size> 
    class SCQueue {
        public:
            SCQueue(void);
            bool enqueue(T item);
            T dequeue(void);
            T peek(void);
            bool isEmpty(void);
            void log_values(void);

        private:
            T queue_[allocation_size];
            size_t first_ = 0;
            size_t last_ = 0;
            const size_t capacity_ = allocation_size;
            bool is_empty_ = true;
            inline void mutexLock(void);
            inline void mutexUnlock(void);
#ifdef SCQ_RP2040_MUTEX
            mutex_t scq_mutex_;
#endif
    };
}

#include "scqueue.tpp"