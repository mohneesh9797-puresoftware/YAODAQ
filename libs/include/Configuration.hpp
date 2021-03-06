#pragma once

#include "BoardInfos.hpp"
#include "ConnectorInfos.hpp"
#include "toml.hpp"

#include <map>
#include <string>
#include <vector>

class Configuration
{
public:
  void           parse();
  std::string    getFileName();
  void           setFileName(const std::string&);
  toml::value    getConfig(const std::string&);
  ConnectorInfos getConnectorInfos(const std::string&);

private:
  static int                            m_ConnectorID;
  int                                   m_CrateConnectorID{0};
  void                                  parseRooms();
  void                                  parseRacks(const toml::value& room);
  void                                  parseCrates(const toml::value& rack);
  void                                  parseModules(const toml::value& crate, const toml::value& connectorParameters, bool haveCrateConnector);
  void                                  throwIfExists(std::vector<std::string>& type, const std::string& typeName, const std::string& name);
  void                                  fillIndexes();
  std::string                           m_Filename{""};
  toml::value                           m_Conf;
  bool                                  m_isParsed{false};
  std::vector<std::string>              m_Room_Names;
  std::vector<std::string>              m_Rack_Names;
  std::vector<std::string>              m_Crate_Names;
  std::vector<std::string>              m_Module_Names;
  std::map<std::string, BoardInfos>     m_BoardsInfos;
  std::map<std::string, toml::value>    m_ModuleConfig;
  std::map<std::string, ConnectorInfos> m_ConnectorInfos;
  std::string                           actualRoomName{""};
  std::string                           actualRackName{""};
  std::string                           actualCrateName{""};
};
