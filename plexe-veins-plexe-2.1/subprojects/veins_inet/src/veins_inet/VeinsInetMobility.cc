//
// Copyright (C) 2006-2018 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

//
// Veins Mobility module for the INET Framework
// Based on inet::MovingMobilityBase of INET Framework v4.0.0
//

#undef INET_IMPORT
#include "inet/common/INETMath.h"
#include "veins_inet/VeinsInetMobility.h"
#include "inet/visualizer/mobility/MobilityCanvasVisualizer.h"
#include "inet/common/Units.h"
#include "inet/common/geometry/common/GeographicCoordinateSystem.h"

namespace Veins {

using namespace inet::units::values;

Register_Class(VeinsInetMobility);

VeinsInetMobility::VeinsInetMobility()
{
}

VeinsInetMobility::~VeinsInetMobility()
{
}

void VeinsInetMobility::preInitialize(std::string external_id, const inet::Coord& position, std::string road_id, double speed, double angle)
{
    Enter_Method_Silent();
    this->external_id = external_id;
    lastPosition = position;
    lastVelocity = inet::Coord(cos(angle), -sin(angle)) * speed;
    lastOrientation.alpha = rad(-angle);
}

void VeinsInetMobility::initialize(int stage)
{
    MobilityBase::initialize(stage);

    // We patch the OMNeT++ Display String to set the initial position. Make sure this works.
    ASSERT(hasPar("initFromDisplayString") && par("initFromDisplayString"));
}

void VeinsInetMobility::nextPosition(const inet::Coord& position, std::string road_id, double speed, double angle)
{
    Enter_Method_Silent();

    lastPosition = position;
    lastVelocity = inet::Coord(cos(angle), -sin(angle)) * speed;
    lastOrientation = inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0));

    emitMobilityStateChangedSignal();
}

inet::Coord VeinsInetMobility::getCurrentPosition()
{
    return lastPosition;
}

inet::Coord VeinsInetMobility::getCurrentVelocity()
{
    return lastVelocity;
}

inet::Coord VeinsInetMobility::getCurrentAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

inet::EulerAngles VeinsInetMobility::getCurrentAngularPosition()
{
    return lastOrientation;
}

inet::EulerAngles VeinsInetMobility::getCurrentAngularVelocity()
{
    return lastAngularVelocity;
}

inet::EulerAngles VeinsInetMobility::getCurrentAngularAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

void VeinsInetMobility::setInitialPosition()
{
    subjectModule->getDisplayString().setTagArg("p", 0, lastPosition.x);
    subjectModule->getDisplayString().setTagArg("p", 1, lastPosition.y);
    MobilityBase::setInitialPosition();
}

void VeinsInetMobility::handleSelfMessage(cMessage* message)
{
}

std::string VeinsInetMobility::getExternalId() const
{
    if (external_id == "") throw cRuntimeError("TraCIMobility::getExternalId called with no external_id set yet");
    return external_id;
}

TraCIScenarioManager* VeinsInetMobility::getManager() const
{
    if (!manager) manager = TraCIScenarioManagerAccess().get();
    return manager;
}

TraCICommandInterface* VeinsInetMobility::getCommandInterface() const
{
    if (!commandInterface) commandInterface = getManager()->getCommandInterface();
    return commandInterface;
}

TraCICommandInterface::Vehicle* VeinsInetMobility::getVehicleCommandInterface() const
{
    if (!vehicleCommandInterface) vehicleCommandInterface = new TraCICommandInterface::Vehicle(getCommandInterface()->vehicle(getExternalId()));
    return vehicleCommandInterface;
}

} // namespace Veins
