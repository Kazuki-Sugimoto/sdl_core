/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_RPC_PLUGIN_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_RPC_PLUGIN_H
#include "application_manager/command_factory.h"
#include "application_manager/plugin_manager/rpc_plugin.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;
namespace plugins = application_manager::plugin_manager;

class SystemCapabilityAppExtension;

class SDLRPCPlugin : public plugins::RPCPlugin {
  // RPCPlugin interface
 public:
  bool Init(app_mngr::ApplicationManager& app_manager,
            app_mngr::rpc_service::RPCService& rpc_service,
            app_mngr::HMICapabilities& hmi_capabilities,
            policy::PolicyHandlerInterface& policy_handler,
            resumption::LastStateWrapperPtr last_state) OVERRIDE;

  DEPRECATED
  bool Init(app_mngr::ApplicationManager& app_manager,
            app_mngr::rpc_service::RPCService& rpc_service,
            app_mngr::HMICapabilities& hmi_capabilities,
            policy::PolicyHandlerInterface& policy_handler,
            resumption::LastState& last_state) OVERRIDE;

  bool IsAbleToProcess(
      const int32_t function_id,
      const app_mngr::commands::Command::CommandSource message_source) OVERRIDE;
  std::string PluginName() OVERRIDE;

  app_mngr::CommandFactory& GetCommandFactory() OVERRIDE;
  void OnPolicyEvent(plugins::PolicyEvent event) OVERRIDE;
  void OnApplicationEvent(plugins::ApplicationEvent event,
                          app_mngr::ApplicationSharedPtr application) OVERRIDE;

 private:
  void ClearSubscriptions(app_mngr::ApplicationSharedPtr app);

  std::unique_ptr<application_manager::CommandFactory> command_factory_;
};
}  // namespace sdl_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create();
extern "C" void Delete(application_manager::plugin_manager::RPCPlugin* data);
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_RPC_PLUGIN_H
