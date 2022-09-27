/**
 * QuokkADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 */

/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* The ADB code here is mostly derived from: https://github.com/tmk/tmk_keyboard
 * It was originally ADB host, and I've adapted it for device use.
 * Below is the license and credit for the original ADB host implementation.
 */
/*
Copyright 2011 Jun WAKO <wakojun@gmail.com>
Copyright 2013 Shay Green <gblargg@gmail.com>
This software is licensed with a Modified BSD License.
All of this is supposed to be Free Software, Open Source, DFSG-free,
GPL-compatible, and OK to use in both free and proprietary applications.
Additions and corrections to this file are welcome.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.
* Neither the name of the copyright holders nor the names of
  contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

namespace simple_circular_queue {
    
    template <typename T, size_t allocation_size> 
    SCQueue<T, allocation_size>::SCQueue(void) {
        mutex_init(&scq_mutex_);
    }


    template <typename T, size_t allocation_size> 
    bool SCQueue<T, allocation_size>::enqueue(T item) {
        bool enqueued = true;
        
        mutex_enter_blocking(&scq_mutex_);
        size_t new_last = (last_ + 1) % capacity_;
        
        // no overwriting, return false if queue is full
        if (!isEmpty_ && first_ == new_last) {
            enqueued = false;
        }
        else {
            if(isEmpty_) {
                queue_[last_] = item;
                isEmpty_ = false;
            }
            else {
                last_ = new_last;
                queue_[last_] = item;
                isEmpty_ = false;
            }
        }
        mutex_exit(&scq_mutex_);
        return enqueued;
    }
    
    template <typename T, size_t allocation_size> 
    T SCQueue<T, allocation_size>::dequeue(void) {
        // return NULL if no items
        T item = NULL;
        
        mutex_enter_blocking(&scq_mutex_);
        if (!isEmpty_) {
            // last item to dequeue
            if (first_ == last_) {
                isEmpty_ = true;
                item = queue_[first_];
            }
            else {
                size_t old_first = first_;
                first_ = (first_ + 1) % capacity_;
                item = queue_[old_first];
            }
        }
        mutex_exit(&scq_mutex_);
        
        return item;
    }
   
    template <typename T, size_t allocation_size> 
    bool SCQueue<T, allocation_size>::isEmpty(void) {
        return isEmpty_;
    }
        
}