#include <algorithm>
#include <map>
#include <vector>
#include <tuple>
#include "backup_calc.h"

typedef std::vector< std::tuple<int,int> > tuple_vec;

static int available_sz  = 0;
static int curr_min_backup = 0;
static tuple_vec a_matches;
static std::map<int,int> b_ins_map;
static int all_sz = 0;

static bool compareByA(const std::tuple<int,int> &a, const std::tuple<int,int> &b)
{
    return std::get<0>(a) < std::get<0>(b);
}

static void fill_vectors(RbImageDiff* &d_it, int &num_files)
{
	for (RbImageDiff * it = d_it; it< d_it + num_files; it++)
	{
		a_matches.push_back( std::make_tuple( it->a_pos, it->a_pos + diff_entry_get_a_sz(it) ) );
		if (diff_entry_is_insert(it))
			b_ins_map[it->b_pos] = diff_entry_get_b_sz(it);

	}
	std::sort(a_matches.begin(), a_matches.end(), compareByA);
}

static void update_avaible_sz(int &page_sz, int &a_strt, int &a_end, std::map<int,int>::iterator &it_b)
{
	int b_strt, b_end, sz;
	while(it_b !=b_ins_map.end() && it_b->first < a_strt + a_end)
	{
 		b_strt = it_b->first; 
 		b_end = b_strt + it_b->second;
 		sz = MAX(a_strt, b_strt)< MIN(a_end, b_end) ? MIN(a_end, b_end) - MAX(a_strt, b_strt) : 0;
 		it_b++;
 		available_sz += sz/page_sz;
 		all_sz +=sz;
 	}
}

void get_initial_backup(int page_sz, RbImageDiff* d_it, int num_files)
{
	a_matches.clear();
	b_ins_map.clear();
	available_sz = 0;
	curr_min_backup = 0;
	fill_vectors(d_it, num_files);
 	std::map<int,int>::iterator it_b= b_ins_map.begin();
 	int a_strt = 0, a_end = 0;
 	for (tuple_vec::iterator it_a= a_matches.begin(); it_a!=a_matches.end(); ++it_a)
 	{
 		a_end = std::get<0>(*it_a);
 		if (a_end<=a_strt)
 			continue;
 		update_avaible_sz(page_sz, a_strt, a_end, it_b);
 		a_strt = std::get<1>(*it_a);
 	}
 	return;
}

void accumulate_backup(int page_sz, RbImageDiff* d_it)
{
 	int a_strt = d_it->a_pos, a_end = d_it->a_pos + diff_entry_get_a_sz(d_it);
 	std::map<int,int>::iterator it_b= b_ins_map.upper_bound(a_strt);
 	if(it_b!=b_ins_map.begin())
		it_b--;
 	update_avaible_sz(page_sz, a_strt, a_end, it_b);
 	return;
}

void update_cur_min_backup(int cur_min)
{
	int tmp_sz = cur_min>available_sz ? cur_min - available_sz : 0;
	curr_min_backup = tmp_sz>curr_min_backup ? tmp_sz : curr_min_backup;
}

int get_cur_min_backup()
{
	return curr_min_backup;
}
