/*
 * LocalDatabase.h
 *
 *  Created on: 11/11/2010
 *      Author: victor
 */

#ifndef LOCALDATABASE_H_
#define LOCALDATABASE_H_

#include "system/lang.h"

#include "engine/log/Logger.h"

#include "berkley/BerkeleyDatabase.h"

namespace engine {
 namespace db {

 class DatabaseManager;

 class LocalDatabase : public Logger {
 protected:
	 engine::db::berkley::BerkeleyDatabase* objectsDatabase;
	 engine::db::berkley::Environment* environment;

	 String databaseFileName;

 protected:
	 virtual void closeDatabase();
	 virtual void openDatabase(const engine::db::berkley::DatabaseConfig& dbConfig);
	 virtual void openDatabase();

	 LocalDatabase() {
	 }

 public:
	 const static int DEADLOCK_MAX_RETRIES = 1000;

	 LocalDatabase(engine::db::DatabaseManager* dbEnv, const String& dbFileName);
	 virtual ~LocalDatabase();

	 int getData(Stream* inputKey, ObjectInputStream* objectData);

	 //incoming streams will be deleted
	 int putData(Stream* inputKey, Stream* stream);
	 int deleteData(Stream* inputKey);

	 //incoming streams wont be deleted
	 int tryPutData(Stream* inputKey, Stream* stream, engine::db::berkley::Transaction* transaction);
	 int tryDeleteData(Stream* inputKey, engine::db::berkley::Transaction* transaction);

	 int sync();

	 virtual bool isObjectDatabase() {
		 return false;
	 }

	 inline engine::db::berkley::BerkeleyDatabase* getDatabaseHandle() {
		 return objectsDatabase;
	 }

	 inline void getDatabaseName(String& name) {
		 name = databaseFileName.replaceFirst(".db", "");
	 }

 };

 class LocalDatabaseIterator : public Logger {
	 engine::db::berkley::Cursor* cursor;
	 engine::db::berkley::BerkeleyDatabase* databaseHandle;

	 engine::db::berkley::DatabaseEntry key, data;

 public:
	 LocalDatabaseIterator(LocalDatabase* database, bool useCurrentThreadTransaction = false);
	 LocalDatabaseIterator(engine::db::berkley::BerkeleyDatabase* databaseHandle);
	 ~LocalDatabaseIterator();

	 void resetIterator();

	 bool getNextKeyAndValue(ObjectInputStream* keyStream, ObjectInputStream* data);
	 bool getNextValue(ObjectInputStream* data);
	 bool getNextKey(ObjectInputStream* key);

	 inline void closeCursor() {
		 if (cursor != NULL) {
			 cursor->close();

			 delete cursor;
		 }

		 cursor = NULL;
	 }
 };

 }
}

#endif /* LOCALDATABASE_H_ */