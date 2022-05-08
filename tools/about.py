#!/bin/python

# Standard Library
import argparse

# PyGCCXML
from pygccxml import utils
from pygccxml import parser as xml_parser

# About
from impl.generate_meta import generate_meta
from impl.generate_enum_ostream import generate_enum_ostream

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-i",  "--inputs", nargs="*", required=True, help="Input file paths", default=None)
    parser.add_argument("-om", "--output-meta", type=str, help="Output file path", default=None)
    parser.add_argument("-oe", "--output-enum-ostream", type=str, help="Output file path for enum utilities", default=None)
    parser.add_argument("-d",  "--debug", action="store_true", help="Print generated file contents to console")
    args = parser.parse_args()

    # Find the location of the xml generator (castxml or gccxml)
    generator_path, generator_name = utils.find_xml_generator()

    # Configure the xml generator
    xml_generator_config = xml_parser.xml_generator_configuration_t(
        xml_generator_path=generator_path,
        xml_generator=generator_name)

    if (args.output_meta or args.debug):
        generate_meta(args=args, xml_generator_config=xml_generator_config)

    if (args.output_enum_ostream or args.debug):
        generate_enum_ostream(args=args, xml_generator_config=xml_generator_config)
