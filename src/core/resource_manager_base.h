#pragma once

#include "core/pod_hash_map.h"

namespace Lux 
{
	namespace FS
	{
		class FileSystem;
	}

	class Path;
	class Resource;
	class ResourceManager;

	class LUX_CORE_API ResourceManagerBase LUX_ABSTRACT
	{
		typedef PODHashMap<uint32_t, Resource*> ResourceTable;

	public:
		void create(uint32_t id, ResourceManager& owner);
		void destroy(void);

		Resource* get(const Path& path);
		Resource* load(const Path& path);

		void unload(const Path& path);
		void unload(Resource& resource);

		void forceUnload(const Path& path);
		void forceUnload(Resource& resource);

		void reload(const Path& path);
		void reload(Resource& resource);

		void releaseAll(void);

		ResourceManagerBase(void);
		~ResourceManagerBase(void);
	protected:
		virtual Resource* createResource(const Path& path) = 0;
		virtual void destroyResource(Resource& resource) = 0;

		ResourceManager& getOwner() const { return *m_owner; }
	private:

		uint32_t m_size;
		ResourceTable m_resources;
		ResourceManager* m_owner;
	};
}