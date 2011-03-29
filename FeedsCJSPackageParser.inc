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

    // Find the path to the package.json file
    $package = drupal_realpath($fetcher_result->repository_path . "/package.json");
    $base = $this->similarFileExists($package);
    if($base === FALSE) {
      return array();
    }

    // Parse and decode the file
    $package = drupal_realpath($fetcher_result->repository_path . "/" . $base);
    $object = json_decode(file_get_contents($package));

    if($object == NULL || sizeof($object) == 0) {
      return array();
    }

    $result = new FeedsParserResult();
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
