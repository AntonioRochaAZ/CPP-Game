# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'CPP Game'
copyright = '2024, Antonio ROCHA'
author = 'Antonio ROCHA'
release = '2024'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "breathe", 'sphinx.ext.autodoc', 'sphinx.ext.napoleon', 'sphinx.ext.autosectionlabel', 
    'nbsphinx', 'sphinx.ext.viewcode', 'nbsphinx_link', "sphinx.ext.imgmath"
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_title = 'CPP Game'
html_theme = 'furo'
html_static_path = ['_static']

# BREATHE:

breathe_projects = {"CPP_Game": "../doxygen/xml"}
breathe_default_project = "CPP_Game"
