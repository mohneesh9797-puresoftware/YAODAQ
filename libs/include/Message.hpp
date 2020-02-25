#pragma once

#include "json.h"

#include <memory>
#include <set>

class Message
{
public:
  Message(const std::string& type = "Message", const std::string& content = "", const std::string& to = "", const std::string& from = "");
  void         parse(const std::string&);
  void         setFrom(const std::string&);
  void         setTo(const std::string&);
  virtual void setContent(const std::string&);
  std::string  getFrom() const;
  std::string  getTo() const;
  std::string  getContent() const;
  std::string  get() const;
  std::string  getType() const;
  std::string  print() const;
  std::string  getFrom();
  std::string  getTo();
  std::string  getContent();
  std::string  get();
  std::string  getStyled(const std::string& indent = "\t");
  std::string  getType();
  void         print(const std::string& indent = "\t");

protected:
  void setType(const std::string&);

private:
  static Json::StreamWriterBuilder    m_StreamWriterBuilder;
  std::unique_ptr<Json::StreamWriter> m_Writer{nullptr};
  std::unique_ptr<Json::CharReader>   m_Reader{nullptr};
  static Json::CharReaderBuilder      m_CharReaderBuilder;
  Json::Value                         m_Value{};
};

class Info: public Message
{
public:
  Info(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Log: public Message
{
public:
  Log(const std::string& content = "", const std::string& to = "", const std::string& from = "");
};

class Status: public Message
{
public:
  Status(const std::string& content = "", const std::string& to = "", const std::string& from = "");
  virtual void setContent(const std::string&) final;

private:
  static std::set<std::string> m_Status;
  std::string                  checkContent(const std::string& content);
};