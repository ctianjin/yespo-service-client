#ifndef MODELGEN_INPUT_HPP
#define MODELGEN_INPUT_HPP

#include <cstddef>
#include <iosfwd>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <boost/filesystem/path.hpp>
#include "model.hpp"

namespace modelgen {

class input
{
public:
    input();
    ~input();

    bool verbose;
    bool check_primary_key;
    boost::filesystem::path input_dir;
    boost::filesystem::path input_file;

    void process(database& db);

private:
    void process_attr(std::size_t line_no, const std::string& s, database& db);
    void process_pragma(std::size_t line_no, const std::string& s, database& db);
    void process_field(std::size_t line_no, const std::string& s, field_ptr f);
    void process_enum(std::size_t& line_no, const std::string& s, enumeration_ptr e, std::istream& is);

    static std::pair<std::string, std::string>
    extract_names(std::size_t line_no, const std::string& name, char c1 = '(', char c2 = ')');

    void report_warning(std::size_t line_no, const std::string& msg) const;
    static void report_error(std::size_t line_no, const std::string& msg);
    static void report_bad_format(std::size_t line_no);

private:
    typedef std::map<std::string, int> type_map;
    type_map types_;

    std::string name_;
    model_ptr cur_model_;
};

} // namespace modelgen

#endif // MODELGEN_INPUT_HPP
