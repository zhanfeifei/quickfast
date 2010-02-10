// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MESSAGEPERPACKETQUEUESERVICE_H
#define MESSAGEPERPACKETQUEUESERVICE_H

#include "MessagePerPacketQueueService_fwd.h"
#include <Communication/BufferQueueService.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataSource.h>
#include <Codecs/HeaderAnalyzer.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class MessagePerPacketQueueService
      : public Communication::BufferQueueService
      , public Codecs::DataSource
    {
    public:
      /// @brief Constuct given multicast info
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param builder receives the data from the decoder.
      MessagePerPacketQueueService(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & headerAnalyzer,
          Messages::ValueMessageBuilder & builder);

      virtual ~MessagePerPacketQueueService();

      /// @brief set the maximum number of messages to decode
      /// @param messageLimit is the number of messages to decode
      void setMessageLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      /// @brief Access the internal decoder
      /// @returns a reference to the internal decoder
      Codecs::Decoder & decoder()
      {
        return decoder_;
      }

      /// @brief Access the internal decoder (const)
      /// @returns a const reference to the internal decoder
      const Codecs::Decoder & decoder() const
      {
        return decoder_;
      }

      /// @brief How many messages have been processed
      /// @returns the message count
      size_t messageCount()const
      {
        return messageCount_;
      }

      /// @brief How many bytes have been processed
      /// @returns the byte count
      size_t byteCount()const
      {
        return byteCount_;
      }

      ///////////////////////////
      // Implement BufferQueueService
      virtual void receiverStarted(Communication::Receiver & receiver);
      virtual void receiverStopped(Communication::Receiver & receiver);
      virtual bool serviceQueue(Communication::Receiver & receiver);

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      bool consumeBuffer(const unsigned char * buffer, size_t size);
    private:
      MessagePerPacketQueueService & operator = (const MessagePerPacketQueueService &);
      MessagePerPacketQueueService(const MessagePerPacketQueueService &);
      MessagePerPacketQueueService();

    private:
      HeaderAnalyzer & headerAnalyzer_;
      Messages::ValueMessageBuilder & builder_;
      Decoder decoder_;
      const unsigned char * buffer_;
      size_t size_;

      size_t messageCount_;
      size_t byteCount_;
      size_t messageLimit_;
    };
  }
}
#endif // MESSAGEPERPACKETQUEUESERVICE_H
