/*
** Copyright (C) 2007-2019 SWGEmu
** See file COPYING for copying conditions.
*/
#ifndef CRYPTO_H_
#define CRYPTO_H_

#include "system/platform.h"
#include "system/lang/String.h"
#include "system/lang/System.h"

#include <limits>
#include <cstddef>

namespace engine {
	namespace log {
		class Logger;
	}
}

namespace sys {
	namespace security {
		class Crypto {
		public:
			static String MD5Hash(const String& str);
			static String SHA1Hash(const String& str);
			static String SHA256Hash(const String& str);
			static String SHA512Hash(const String& str);

			static uint64 randomOpenSSLBytes(uint8* bytes, std::size_t size);

			template<std::size_t StringLength = 32>
			static String randomSalt();

			static String hashToString(uint8* val, std::size_t size);

		};
	}

	template<std::size_t StringLength>
	String sys::security::Crypto::randomSalt() {
		constexpr auto bufferBytes = Math::max(static_cast<std::size_t>(1), StringLength / 2);

		uint8 buffer[bufferBytes];

		uint64 res = Crypto::randomOpenSSLBytes(buffer, bufferBytes);

		if (res) {
			static engine::log::Logger logger("Crypto");

			logger.warning() << "OpenSSL could not generate random bytes, using mt19937";

			for (std::size_t i = 0; i < bufferBytes; ++i) {
				buffer[i] = System::random(std::numeric_limits<uint8_t>::max());
			}
		}

		return hashToString(buffer, bufferBytes);
	}
}

using namespace sys::security;

#endif /* CRYPTO_H_ */
