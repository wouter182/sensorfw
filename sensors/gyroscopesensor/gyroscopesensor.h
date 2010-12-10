/**
   @file gyroscopesensor.h
   @brief GyroscopeSensor

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Timo Rongas <ext-timo.2.rongas@nokia.com>
   @author Samuli Piippo <ext-samuli.1.piippo@nokia.com>

   This file is part of Sensord.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */

#ifndef GYROSCOPE_SENSOR_CHANNEL_H
#define GYROSCOPE_SENSOR_CHANNEL_H

#include "abstractsensor.h"
#include "sensord/deviceadaptor.h"


// Include the dbus adaptor and dbusemitter base class
#include "gyroscopesensor_a.h"
#include "sensord/dbusemitter.h"

// Include required datatypes
#include "datatypes/orientationdata.h"
#include "datatypes/xyz.h"

// Included in .cpp, introduced here.
class Bin;
template <class TYPE> class BufferReader;
class FilterBase;

// Sensors inherit also DbusEmitter
class GyroscopeSensorChannel :
        public AbstractSensorChannel,
        public DbusEmitter<AngularVelocityData>
{
    Q_OBJECT;

    // Accessor function for the value for clients. Type must be based
    // on QObject and marshallable over D-Bus. Usually it just wraps the
    // POD that is used internally.
    Q_PROPERTY(XYZ value READ get);

public:
    /**
     * Factory method for GyroscopeSensorChannel.
     * @return new GyroscopeSensorChannel as AbstractSensorChannel*.
     */
    static AbstractSensorChannel* factoryMethod(const QString& id)
    {
        GyroscopeSensorChannel* sc = new GyroscopeSensorChannel(id);

        // This creates the dbus adaptor - qobject parent relation
        // makes sure it gets destructed correctly.
        new GyroscopeSensorChannelAdaptor(sc);

        return sc;
    }

    // Implementation of the accessor function.
    XYZ get() const { return previousSample_; }

public Q_SLOTS:
    bool start();
    bool stop();

signals:
    // Signal is used whenever we have new data.
    void dataAvailable(const AngularVelocityData& data);

protected:
    GyroscopeSensorChannel(const QString& id);
    ~GyroscopeSensorChannel();

private:
    Bin*                         filterBin_;
    Bin*                         marshallingBin_;

    // Pointer to chain and a reader for it.
    DeviceAdaptor*                     gyroscopeAdaptor_;
    BufferReader<AngularVelocityData>* gyroscopeReader_;
    RingBuffer<AngularVelocityData>*   outputBuffer_;

    // We need to store the previous sample for accessor.
    AngularVelocityData                previousSample_;

    // Function that takes care of pushing the data to clients
    void emitToDbus(const AngularVelocityData& value);
};

#endif // GYROSCOPE_SENSOR_CHANNEL_H