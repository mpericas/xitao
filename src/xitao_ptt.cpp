#include "xitao_ptt.h"


tmap xitao_ptt::runtime_ptt_tables;

ptt_shared_type xitao_ptt::try_insert_table(PolyTask* pt, size_t const& workload_hint) {
  // declare the tao_info to capture its type
  xitao_ptt_key tao_info (workload_hint, typeid(*pt));

  // the PTT table 
  ptt_shared_type _ptt;

  // check if entry is new
  if(runtime_ptt_tables.find(tao_info) == runtime_ptt_tables.end()) {
    // allocate the ptt table and place it in shared pointer
    _ptt = std::make_shared<ptt_value_type>(XITAO_MAXTHREADS * XITAO_MAXTHREADS, 0);
    // insert the ptt table to the mapper
    runtime_ptt_tables.insert(std::make_pair(tao_info, _ptt));

  } else { 
    // get the existing value
    _ptt = runtime_ptt_tables[tao_info];

  } 
  // return the newly created or existing ptt table
  return _ptt;
}

#ifdef TRACK_STA
sta_sched_map_type xitao_ptt::schedule_table;

  void xitao_ptt::try_insert_schedule_map(PolyTask* pt, size_t const& workload_hint) {
    // declare the tao_info to capture its type
    xitao_ptt_key tao_info (workload_hint, typeid(*pt));

    sta_sched_entry_type_ptr _sta_tracker;
    // check if entry is new
    if(schedule_table.find(tao_info) == schedule_table.end()) {
      // allocate the sta scheduler tracker
      _sta_tracker = std::make_shared<sta_sched_entry_type>();
      schedule_table.insert(std::make_pair(tao_info, _sta_tracker));
    }
  }

  void xitao_ptt::update_schedule(PolyTask* pt, uint32_t tid){
    // declare the tao_info to capture its type
    xitao_ptt_key tao_info (pt->workload_hint, typeid(*pt));
    auto&& entries = schedule_table[tao_info];
    entries->push_back(tid);
  }

#endif

void xitao_ptt::clear_tables() {
  // iterate over all ptts in the hashmap to release the ownership of all PTT pointers

  for(auto&& ptt : runtime_ptt_tables) {

    // release the ownership of the ptt table 
    // remember that the TAO objects still have handles
    // and that they must be freed/deleted/scoped out for the smart pointer to be freed
    ptt.second.reset(); 
  }

  // clear the hash map
  runtime_ptt_tables.clear();
}

void xitao_ptt::reset_table(PolyTask* pt, size_t const& workload_hint){
 // declare the tao_info to capture its type
  xitao_ptt_key tao_info (workload_hint, typeid(*pt));
  // check if entry is new
  if(runtime_ptt_tables.find(tao_info) != runtime_ptt_tables.end()) {      
    // get the existing value
    ptt_shared_type _ptt = runtime_ptt_tables[tao_info];            
    // reset all PTT values
    std::fill(_ptt->begin(), _ptt->end(), 0);
  } 
}

void xitao_ptt::print_table(ptt_shared_type ptt, const char* table_name) { 
  // capture the underlying table
  auto&& table = *ptt;

  // print table details
  std::cout << std::endl << table_name <<  " PTT Stats: " << std::endl;
  for(int leader = 0; leader < ptt_layout.size() && leader < gotao_nthreads; ++leader) {
    auto row = ptt_layout[leader];
    std::sort(row.begin(), row.end());
    std::ostream time_output (std::cout.rdbuf());
    std::ostream scalability_output (std::cout.rdbuf());
    std::ostream width_output (std::cout.rdbuf());
    std::ostream empty_output (std::cout.rdbuf());
    time_output  << std::scientific << std::setprecision(3);
    scalability_output << std::setprecision(3);    
    empty_output << std::left << std::setw(5);
    std::cout << "Thread = " << leader << std::endl;    
    std::cout << "==================================" << std::endl;
    std::cout << " | " << std::setw(5) << "Width" << " | " << std::setw(9) << std::left << "Time" << " | " << "Scalability" << std::endl;
    std::cout << "==================================" << std::endl;
    for (int i = 0; i < row.size(); ++i) {
      int curr_width = row[i];
      if(curr_width <= 0) continue;
      auto comp_perf = table[(curr_width - 1) * XITAO_MAXTHREADS + leader];
      std::cout << " | ";
      width_output << std::left << std::setw(5) << curr_width;
      std::cout << " | ";      
      time_output << comp_perf; 
      std::cout << " | ";
      if(i == 0) {        
        empty_output << " - ";
      } else if(comp_perf != 0.0f) {
        auto scaling = table[(row[0] - 1) * XITAO_MAXTHREADS + leader]/comp_perf;
        auto efficiency = scaling / curr_width;
        if(efficiency  < 0.6 || efficiency > 1.3) {
          scalability_output << "(" <<table[(row[0] - 1) * XITAO_MAXTHREADS + leader]/comp_perf << ")";  
        } else {
          scalability_output << table[(row[0] - 1) * XITAO_MAXTHREADS + leader]/comp_perf;
        }
      }
      std::cout << std::endl;  
    }
    std::cout << std::endl;
  }
}