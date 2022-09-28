#include "pch.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/cstdlib.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include "model.hpp"
#include "input.hpp"
#include "output.hpp"
#include "version.hpp"

using namespace std;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

static void print_usage()
{
    cout << "Usage: " << MODELGEN_NAME << " [options] file1 [file2] [file3] ..." << endl;
}

static void print_help(const po::options_description& opts)
{
    print_usage();
//    cout << endl;
    cout << opts;
}

static void print_version()
{
    cout << "Model generator, version "
         << MODELGEN_VERSION_STR << endl;
    cout << "Copyright (C) 2014 RAR Limited" << endl;
    cout << "This program is NOT free software." << endl;
}

static int process(int argc, char* argv[])
{
    modelgen::input input;
    modelgen::output output;

    bool verbose = false;
    string config_file;
    string input_dir;
    string output_dir;
    vector<string> models;
    vector<string> input_files;
    vector<string> type_aliases;
    vector<string> type_includes;

    po::options_description generic_opts("Allowed options");
    generic_opts.add_options()
        ("version", "print program version")
        ("help,h", "print this message")
        ("config-file,f", po::value<string>(&config_file), "configuration file")
        ("input-dir,i", po::value<string>(&input_dir)->default_value("."), "input directory")
        ("output-dir,o", po::value<string>(&output_dir)->default_value("."), "output directory")
        ("ignore-custom,c", po::bool_switch(&output.ignore_custom), "ignore customizations")
        ("variant,V", po::bool_switch(&output.variant), "write variant functions")
        ("no-db,D", po::bool_switch(&output.no_db), "disable database class generation")
        ("model,m", po::value<vector<string> >(&models)->composing(), "entity model generation")
        ("no-model,M", po::bool_switch(&output.no_model), "disable entity model generation")
        ("private-dao,O", po::bool_switch(&output.private_dao), "make dao classes private")
        ("pch,H", po::value<string>(&output.pch), "precompiled header filename")
        ("namespace,n", po::value<string>(&output.ns), "namespace of classes (xxx.xxx.xxx)")
        ("decl,d", po::value<string>(&output.decl), "declare specification")
        ("global,g", po::value<string>(&output.global), "global header")
        ("check-primary-key,p", po::bool_switch(&input.check_primary_key), "check primary key of the model")
        ("type-alias,A", po::value<vector<string> >(&type_aliases)->composing(), "type alias (type=alias)")
        ("type-include,I", po::value<vector<string> >(&type_includes)->composing(), "type include (type=include)")
        ("verbose,v", po::bool_switch(&verbose), "verbose mode")
        ;

    po::options_description hidden_opts("Hidden options");
    hidden_opts.add_options()
        ("input-file", po::value<vector<string> >(&input_files)->composing(), "input file")
    ;

    po::options_description cmdline_opts;
    cmdline_opts.add(generic_opts).add(hidden_opts);

    po::positional_options_description pos_opts;
    pos_opts.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(cmdline_opts).positional(pos_opts).run(), vm);
    po::notify(vm);

    if (!config_file.empty()) {
        po::store(po::parse_config_file<char>(config_file.c_str(), cmdline_opts), vm);
        po::notify(vm);
    }

    // usage/help/version messages
    if (vm.count("help")) {
        print_help(generic_opts);
        return boost::exit_success;
    }

    if (vm.count("version")) {
        print_version();
        return boost::exit_success;
    }

    // input file must be specified
    if (!vm.count("input-file")) {
        print_usage();
        return boost::exit_success;
    }

    // verbose
    input.verbose = verbose;
    output.verbose = verbose;

    // input-dir, output-dir
    input.input_dir = input_dir;
    output.output_dir = output_dir;

    // Set models informations
    if (vm.count("model")) {
        output.use_model = true;
    }
    output.models.clear();
    output.models.insert(models.begin(), models.end());

    // Calculate type aliases informations
    output.type_aliases.clear();
    if (!type_aliases.empty()) {
        BOOST_FOREACH(const string& a, type_aliases)
        {
            vector<string> as;
            boost::split(as, a, boost::is_any_of("="));
            if (as.size() != 2) {
                cerr << "Type alias " << a << "is invalid" << endl;
                return boost::exit_failure;
            }
            boost::trim(as[0]);
            boost::trim(as[1]);

            bool ok = false;
            BOOST_FOREACH(const string& i, type_includes)
            {
                vector<string> is;
                boost::split(is, i, boost::is_any_of("="));
                if (is.size() != 2) {
                    cerr << "Type include " << i << "is invalid" << endl;
                    return boost::exit_failure;
                }
                boost::trim(is[0]);
                boost::trim(is[1]);

                if (as[0] == is[0]) {
                    output.type_aliases[as[0]] = make_pair(as[1], is[1]);
                    ok = true;
                    break;
                }
            }

            if (!ok) {
                cerr << "No matching type include for type alias " << as[0];
                return boost::exit_failure;
            }
        }

        if (verbose && !output.type_aliases.empty()) {
            cout << "Type aliases:" << endl;
            BOOST_FOREACH(const modelgen::output::type_alias_map::value_type& val, output.type_aliases)
            {
                cout << "  " << val.first << " = " << val.second.first << ", " << val.second.second << endl;
            }
        }
    }

    // do process, for each input file
    BOOST_FOREACH(const string& s, input_files)
    {
        if (verbose) {
            cout << "======== Begin process file " << s << " ========" << endl;
        }

        try {
            // Make input_file
            fs::path input_file = s;
            if (input_file.is_absolute()) {
                input.input_file = input_file;
            } else {
                input.input_file = input.input_dir / input_file;
            }
            output.input_file = input.input_file;

            modelgen::database db;
            input.process(db);
            output.process(db);

        } catch (const exception& e) {
            cerr << "Error: " << e.what()
                 << " (" << input.input_file.string() << ")" << endl;
        } catch (...) {
            cerr << "Error: unknown ("
                 << input.input_file.string() << ")" << endl;
        }

        if (verbose) {
            cout << "======== End process file " << s << " ========" << endl;
        }
    }

    return boost::exit_success;
}

int main(int argc, char* argv[])
{
    int ret = boost::exit_success;

    try {
        ret = process(argc, argv);

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        ret = boost::exit_failure;
    } catch (...) {
        cerr << "Error: unknown" << endl;
        ret = boost::exit_failure;
    }

    return ret;
}
