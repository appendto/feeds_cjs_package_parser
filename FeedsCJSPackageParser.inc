<?php
// $Id: FeedsCJSPackageParser.inc,v 1.4.2.3 2010/10/25 23:59:31 alexb Exp $

/**
 * A parser for the Sitemap specification http://www.sitemaps.org/protocol.php
 */
class FeedsCJSPackageParser extends FeedsParser {
  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $result = new FeedsParserResult();
    $result->items = array();

    // Find the path to the package.json file
    $package = drupal_realpath($fetcher_result->repository_path . "/package.json");
    $base = $this->similarFileExists($package);
    if($base === FALSE) {
      return $result;
    }

    // Parse and decode the file
    $package = drupal_realpath($fetcher_result->repository_path . "/" . $base);
    $package_contents = file_get_contents($package);
    $object = json_decode($package_contents, TRUE);

    if($object == NULL || sizeof($object) == 0) {
      drupal_set_message("Parse Error or package.json is empty", "warning");
      return $result;
    }

    $result->items[] = $object;

    return $result;
  }

  /**
   * Implements FeedsParser::getMappingSources().
   */
  public function getMappingSources() {
    return array(
      'name' => array(
        'name' => t('Name'),
        'description' => t('The name of the package. This must be a unique, lowercase alpha-numeric name without spaces. It may include "." or "_" or "-" characters. It is otherwise opaque.'),
      ),
      'description' => array(
        'name' => t('Description'),
        'description' => t('A brief description of the package. By convention, the first sentence (up to the first ". ") should be usable as a package title in listings.'),
      ),
      'version' => array(
        'name' => t('Version'),
        'description' => t('A version string conforming to the Semantic Versioning requirements.'),
      ),
      'keywords' => array(
        'name' => t('Keywords'),
        'description' => t('An Array of string keywords to assist users searching for the package in catalogs.'),
      ),
      'maintainers' => array(
        'name' => t('Maintainers'),
        'description' => t('Array of maintainers of the package. Each maintainer is a hash which must have a "name" property and may optionally provide "email" and "web" properties.'),
      ),
      'contributors' => array(
        'name' => t('Contributors'),
        'description' => t('An Array of hashes each containing the details of a contributor. Format is the same as for author. By convention, the first contributor is the original author of the package.'),
      ),
      'bugs' => array(
        'name' => t('Bugs'),
        'description' => t('URL for submitting bugs. Can be mailto or http.'),
      ),
      'licenses' => array(
        'name' => t('Licenses'),
        'description' => t('Array of licenses under which the package is provided. Each license is a hash with a "type" property specifying the type of license and a url property linking to the actual text.'),
      ),
      'repositories' => array(
        'name' => t('Repositories'),
        'description' => t('Array of repositories where the package can be located. Each repository is a hash with properties for the "type" and "url" location of the repository to clone/checkout the package. A "path" property may also be specified to locate the package in the repository if it does not reside at the root.'),
      ),
      'dependencies' => array(
        'name' => t('Dependencies'),
        'description' => t('Hash of prerequisite packages on which this package depends in order to install and run. Each dependency defines the lowest compatible MAJOR[.MINOR[.PATCH]] dependency versions (only one per MAJOR version) with which the package has been tested and is assured to work. The version may be a simple version string (see the version property for acceptable forms), or it may be a hash group of dependencies which define a set of options, any one of which satisfies the dependency. The ordering of the group is significant and earlier entries have higher priority.'),
      ),
      'homepage' => array(
        'name' => t('Home Page'),
        'description' => t('URL string for the package web site.'),
      ),
      'os' => array(
        'name' => t('OS'),
        'description' => t('Array of supported operating systems. If absent or set to the empty set, the package makes no platform assumptions. The set of valid os names includes: aix, freebsd, linux, macos, solaris, vxworks, windows.'),
      ),
      'cpu' => array(
        'name' => t('CPU'),
        'description' => t('Array of supported CPU architectures. If absent or set to the empty set, the package makes no platform assumptions. The set of valid cpu names includes: arm, mips, ppc, sparc, x86, x86_64.'),
      ),
      'engine' => array(
        'name' => t('Engine'),
        'description' => t('Array of supported JavaScript engines. If absent or set to the empty set, the package makes no platform assumptions. The set of valid engine names includes: ejs, flusspferd, gpsee, jsc, spidermonkey, narwhal, node, rhino, v8.'),
      ),
      'builtin' => array(
        'name' => t('Built-In'),
        'description' => t('Boolean value indicating the package is built in as a standard component of the underlying platform'),
      ),
      'directories' => array(
        'name' => t('Directories'),
        'description' => t('Object hash of package directories. Typical directories include "lib", "src", "doc", "jars", "test" and "bin". Package manager tools must use these directory definitions to find various package components.'),
      ),
      'implements' => array(
        'name' => t('Implements'),
        'description' => t('Array of relevant CommonJS specifications this package supports. A specification identifier is the WikiName of the specification prefixed by "CommonJS/". Arbitrary URLs may also be specified to indicate support for externally published specifications.'),
      ),
      'scripts' => array(
        'name' => t('Scripts'),
        'description' => t('Object hash of scripts used in managing the package. A package manager tool may use these scripts to install, build, test or uninstall the package.'),
      ),
    ) + parent::getMappingSources();
  }

  // Thanks to http://us3.php.net/manual/en/function.file-exists.php#90256
  function similarFileExists($filename) {
    if (file_exists($filename)) {
      return basename($filename);
    }

    $dir = dirname($filename);
    $files = glob($dir . '/*');
    $lcaseFilename = strtolower($filename);
    foreach($files as $file) {
      if (strtolower($file) == $lcaseFilename) {
        return $file;
      }
    }
    return false;
  }
}
