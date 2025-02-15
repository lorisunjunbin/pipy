/*
 *  Copyright (c) 2019 by flomesh.io
 *
 *  Unless prior written consent has been obtained from the copyright
 *  owner, the following shall not be allowed.
 *
 *  1. The distribution of any source codes, header files, make files,
 *     or libraries of the software.
 *
 *  2. Disclosure of any source codes pertaining to the software to any
 *     additional parties.
 *
 *  3. Alteration or removal of any notices in or on the software or
 *     within the documentation included within the software.
 *
 *  ALL SOURCE CODE AS WELL AS ALL DOCUMENTATION INCLUDED WITH THIS
 *  SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION, WITHOUT WARRANTY OF ANY
 *  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef WORKER_HPP
#define WORKER_HPP

#include "pjs/pjs.hpp"
#include "context.hpp"

#include <set>
#include <vector>

namespace pipy {

class Module;
class Listener;
class Pipeline;
class Task;

//
// Worker
//

class Worker {
public:
  static auto make() -> Worker* {
    return new Worker();
  }

  static auto current() -> Worker* {
    return s_current;
  }

  auto root() const -> Module* { return m_root; }
  auto global_object() const -> pjs::Object* { return m_global_object; }
  auto get_module(pjs::Str *filename) -> Module*;
  auto find_module(const std::string &path) -> Module*;
  auto load_module(const std::string &path) -> Module*;
  void add_listener(Listener *listener, Pipeline *pipeline, int max_connections);
  void add_task(Task *task);
  void add_export(pjs::Str *ns, pjs::Str *name, Module *module);
  auto get_export(pjs::Str *ns, pjs::Str *name) -> Module*;
  auto new_loading_context() -> Context*;
  auto new_runtime_context(Context *base = nullptr) -> Context*;
  bool start();
  void stop();

private:
  Worker();
  ~Worker();

  typedef pjs::PooledArray<pjs::Ref<pjs::Object>> ContextData;
  typedef std::map<pjs::Ref<pjs::Str>, Module*> Namespace;

  struct ListeningPipeline {
    pjs::Ref<Pipeline> pipeline;
    int max_connections;
  };

  Module* m_root = nullptr;
  pjs::Ref<pjs::Object> m_global_object;
  std::map<std::string, Module*> m_module_map;
  std::map<pjs::Ref<pjs::Str>, Module*> m_module_name_map;
  std::vector<Module*> m_modules;
  std::map<Listener*, ListeningPipeline> m_listeners;
  std::set<Task*> m_tasks;
  std::map<pjs::Ref<pjs::Str>, Namespace> m_namespaces;

  static Worker* s_current;
};

} // namespace pipy

#endif // WORKER_HPP