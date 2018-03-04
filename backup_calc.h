#ifndef __BACKUP_CALC_H
#define __FILE_SYS_IMAGE_STATS_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void get_initial_backup(int page_sz, RbImageDiff* d_it, int num_files);
void accumulate_backup(int page_sz, RbImageDiff* d_it);
void update_cur_min_backup(int cur_min);
int get_cur_min_backup();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif