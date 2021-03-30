/*
* Copyright 2020 The casbin Authors. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "pch.h"

#ifndef TICKER_CPP
#define TICKER_CPP

#include "./ticker.h"

Ticker::Ticker(function<void()> onTick, chrono::duration<int64_t, nano> tickInterval)
    : _onTick (onTick)
    , _tickInterval (tickInterval)
    , _running (false) {}

Ticker::~Ticker () {
    stop();
}

void Ticker::start() {
    if (_running) return;
    _running = true;
    _futures1.push_back(async(launch::async, &Ticker::timer_loop, this));
}

void Ticker::stop() { 
    _running = false; 
}

void Ticker::timer_loop()
{
    while (_running) {
        {
            lock_guard<mutex> lock(_tickIntervalMutex);
            _futures2.push_back(async(launch::async, _onTick));
            this_thread::sleep_for( _tickInterval );
        }
    }
}

#endif // TICKER_CPP
