/**
 * \file appMain.cpp
 * \brief AppLink main application sources
 * \author AKara
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include "appMain.hpp"
#include "CBTAdapter.hpp"

#include "ProtocolHandler.hpp"

#include "JSONHandler/JSONHandler.h"

#include "AppMgr/AppMgr.h"

#include "CMessageBroker.hpp"

//#include "mb_tcpserver.hpp"
/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv)
{
    /*** Components instance section***/
    /**********************************/
    NsTransportLayer::CBTAdapter btadapter;

    JSONHandler jsonHandler;

    AxisCore::ProtocolHandler protocolHandler =  AxisCore::ProtocolHandler(&jsonHandler, &btadapter);

    jsonHandler.setProtocolHandler(&protocolHandler);

    NsAppManager::AppMgr::setParams(std::string("127.0.0.1"), 8087, std::string("AppMgr"));

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

    jsonHandler.setRPCMessagesObserver(&appMgr);

    NsMessageBroker::CMessageBroker *pMessageBroker = NsMessageBroker::CMessageBroker::getInstance();
    if (!pMessageBroker)
    {
        printf("Wrong pMessageBroker pointer!\n");
        return EXIT_SUCCESS;
    }

//    NsMessageBroker::TcpServer *pJSONRPC20Server = new NsMessageBroker::TcpServer(std::string("127.0.0.1"), 8087, pMessageBroker);
//    if (!pJSONRPC20Server)
//    {
//        printf("Wrong pJSONRPC20Server pointer!\n");
//        delete pMessageBroker;
//        return EXIT_SUCCESS;
//    }

    /**********************************/


    /*** Start BT Devices Discovery***/

    std::vector<NsTransportLayer::CBTDevice> devicesFound;
    btadapter.scanDevices(devicesFound);
    if (0 < devicesFound.size())
    {
        printf("Found %d devices\n", devicesFound.size());
        printf("Please make your choice, 0 for exit:\n");
        printf("\n");
    } else
    {
        printf("No any devices found!\n");
        return EXIT_SUCCESS;
    }

    std::vector<NsTransportLayer::CBTDevice>::iterator it;
    int i = 1;
    for(it = devicesFound.begin(); it != devicesFound.end(); it++)
    {
        NsTransportLayer::CBTDevice device = *it;
        printf("%d: %s %s \n", i++, device.getDeviceAddr().c_str(), device.getDeviceName().c_str());
    }

    std::cin >> i;
    std::string discoveryDeviceAddr = "";
    if ((0 < i) && (i <= devicesFound.size()))
    {
        discoveryDeviceAddr = devicesFound[i-1].getDeviceAddr();
    } else
    {
        printf("Exit!\n");
        return EXIT_SUCCESS;
    }

    /*** Start SDP Discovery on device***/

    std::vector<int> portsRFCOMMFound;
    btadapter.startSDPDiscoveryOnDevice(discoveryDeviceAddr.c_str(), portsRFCOMMFound);
    if (0 < portsRFCOMMFound.size())
    {
        printf("Found %d ports on %s device\n", portsRFCOMMFound.size(), discoveryDeviceAddr.c_str());
        printf("Please make your choice, 0 for exit:\n");
    } else
    {
        printf("No any ports discovered!\n");
        return EXIT_SUCCESS;
    }

    std::vector<int>::iterator itr;
    int j = 1;
    for(itr = portsRFCOMMFound.begin(); itr != portsRFCOMMFound.end(); itr++)
    {
        printf("%d: %d \n", j++, *itr);
    }

    std::cin >> j;
    int portRFCOMM = 0;
    if ((0 < j) && (j <= portsRFCOMMFound.size()))
    {
        portRFCOMM = portsRFCOMMFound[j-1];
    } else
    {
        printf("Exit!\n");
        return EXIT_SUCCESS;
    }

    /*** Start RFCOMM connection***/

    int sockID = btadapter.startRFCOMMConnection(discoveryDeviceAddr.c_str(), portRFCOMM);

    if (0 < sockID)
    {
        btadapter.processRFCOMM(sockID);
    }

    return EXIT_SUCCESS;
} 