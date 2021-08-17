#!/bin/sh

gawk '
BEGIN {
	# 前提条件
	#   1) 各FITモジュールフォルダ上から実行される
	#   2) 各FITモジュールにはMakefileが存在しており、そこにバージョン情報が書いてある
	#   3) 各FITモジュールの2階層上位にtoolsフォルダがあり、本ファイルはそこに格納される
	# 
	# 引数
	#   1) FITモジュール名とバージョン情報が書いてあるFITモジュール毎のMakefileのパス情報
	#   2) $(release_tag_commit_number) 例: 8dadb91c0486eaa03af224c935eedb9bb2df69a3
	#     -> v1.25時のコミット番号
	#       https://github.com/renesas/rx-driver-package/releases/tag/V1.25
	#
	# 実行例
	#   sh ../../tools/version_xml_generator.sh Makefile release_tag_commit_number=8dadb91c0486eaa03af224c935eedb9bb2df69a3
	
	# 定数データ
	rdp_repo_url = "https://raw.githubusercontent.com/renesas/rx-driver-package"
	
	while(1)
	{
		if(ARGC != 4)
		{
			printf("error: 3 aruguments are needed. arg number = %d\n", ARGC - 1);
			for(i=1; i<ARGC; i++)
			{
				printf("arg[%d]=%s\n",i, ARGV[i]);
			}
			break;
		}
		
		if(0 == getline)
		{
			/* end of file */
			break;
		}
		
		if($1 == "MODULE_NAME")
		{
			module_name = $3;
#			printf("module_name found: %s\n", module_name);
		}

		if($1 == "VERSION")
		{
			module_version = $3;
#			printf("module_version found: %s\n", module_version);
		}
		
		if($1 == "MDF")
		{
			if($3 == "NOT_SUPPORTED")
			{
				mdf_support_flag = 0;
			}
			else if($3 == "V1")
			{
				mdf_support_flag = 1;
			}
			else if($3 == "V2")
			{
				mdf_support_flag = 2;
			}
			else if($3 == "V1,V2")
			{
				mdf_support_flag = 3;
			}
			else
			{
				mdf_support_flag = 0;
			}
		}
		parse_ok_flag = 1;
	}
	
	if(parse_ok_flag == 1)
	{
		input_release_tag_commit_number_string = ARGV[2];
		input_release_tag_commit_number_depth = split(input_release_tag_commit_number_string, input_release_tag_commit_number_string_array, /=/);
		release_tag_commit_number = input_release_tag_commit_number_string_array[input_release_tag_commit_number_depth];
	#	printf("module commit number found: %s\n", release_tag_commit_number);
		input_release_tag_commit_version_string = ARGV[3];
		input_release_tag_commit_version_depth = split(input_release_tag_commit_version_string, input_release_tag_commit_version_string_array, /=/);
		release_tag_commit_version = input_release_tag_commit_version_string_array[input_release_tag_commit_version_depth];

		zip_file_name = sprintf("%s_v%s.zip", module_name, module_version);
	#	printf("%s\n", zip_file_name);

		xml_file_name = sprintf("%s_v%s.xml", module_name, module_version);
	#	printf("%s\n", xml_file_name);

		if(mdf_support_flag == 1)
		{
			mdf_file_name = sprintf("%s_v%s.mdf", module_name, module_version);
		}
		else if(mdf_support_flag == 2)
		{
			mdf_file_name = sprintf("%s_v%s_extend.mdf", module_name, module_version);
		}
		else if(mdf_support_flag == 3)
		{
			mdf_file_name_v1 = sprintf("%s_v%s.mdf", module_name, module_version);
			mdf_file_name_v2 = sprintf("%s_v%s_extend.mdf", module_name, module_version);
		}
		else
		{
			mdf_file_name = "";
		}
	#	printf("%s\n", mdf_file_name);

		depth = 1;
		
		print_indent(depth);
		# printf("<module>\r\n");
		printf("<module rdp_version=\"%s\">\r\n", release_tag_commit_version);
		depth++;
		
		print_indent(depth);
		printf("<name>");
		printf("%s", module_name);
		printf("</name>\r\n");

		print_indent(depth);
		printf("<version>");
		printf("%s", module_version);
		printf("</version>\r\n");
		
		print_indent(depth);
		printf("<urls>\r\n");
		depth++;

		print_indent(depth);
		printf("<url>%s/%s/FITModules/%s</url>\r\n", rdp_repo_url, release_tag_commit_number, zip_file_name);
		print_indent(depth);
		printf("<url>%s/%s/FITModules/%s</url>\r\n", rdp_repo_url, release_tag_commit_number, xml_file_name);
		if(mdf_support_flag != 0)
		{
			if(mdf_support_flag == 3)
			{
				print_indent(depth);
				printf("<url>%s/%s/FITModules/%s</url>\r\n", rdp_repo_url, release_tag_commit_number, mdf_file_name_v1);
				print_indent(depth);
				printf("<url>%s/%s/FITModules/%s</url>\r\n", rdp_repo_url, release_tag_commit_number, mdf_file_name_v2);
			}
			else
			{
				print_indent(depth);
				printf("<url>%s/%s/FITModules/%s</url>\r\n", rdp_repo_url, release_tag_commit_number, mdf_file_name);
			}
		}
		depth--;
		
		print_indent(depth);
		printf("</urls>\r\n");
		depth--;
		
		print_indent(depth);
		printf("</module>\r\n");
		depth--;
	}
}
function print_indent(depth)
{
    # if文の深さによりインデント調整
    for(j=0; j<depth; j++) {
        printf("  ");
    }
}
' $*
