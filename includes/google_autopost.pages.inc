<?php

/**
 * @file
 * Page Callback file for Google+ Autopost.
 */

/**
 * Page Callback for settings page..
 */
function google_autopost_settings($form, &$form_states) {
  
  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
    '#description' => t('Fill in the information form as specified in <a href="@link">Google Developer Console</a>.',
        array('@link' => 'https://console.developers.google.com/apis/credentials')),
  );
  
  // Client ID.
  $form['credentials']['google_autopost_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#description' => t('OAuth 2.0 client ID'),
    '#default_value' => variable_get('google_autopost_client_id', ''),
    '#required' => TRUE,
  );
  
  // Client Secret.
  $form['credentials']['google_autopost_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#description' => t('OAuth 2.0 client secret'),
    '#default_value' => variable_get('google_autopost_client_secret', ''),
    '#required' => TRUE,
  );
  
  // Callback URI.
  $form['credentials']['google_autopost_callback_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Callback URI'),
    '#description' => t('OAuth 2.0 callback URI.'),
    '#default_value' => variable_get('google_autopost_callback_uri', ''),
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}

/**
 * Page Callback for authentication page.
 */
function google_autopost_auth_form($form, &$form_states) {
  
  // Get Client ID from variable table.
  $client_id = variable_get('google_autopost_client_id', NULL);
  
  // Get Client Secret from variable table.
  $client_secret = variable_get('google_autopost_client_secret', NULL);
  
  // Get Callback URI from variable table.
  $callback_uri = variable_get('google_autopost_callback_uri', NULL);
  
  // Check if client id OR client secret value has been not been set.
  if (!isset($client_id) || !isset($client_secret) || !isset($callback_uri)) {
    // If not then return with information to set the credentials first before
    // proceding further.
    $form['info'] = array(
      '#type' => 'item',
      '#title' => t('Note'),
      '#markup' => t('You need to fill Google+ Domain API Web Application Credentials settings before proceding with authenticaiton.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }
  else {
    // Get token information form variable table.
    $token = variable_get('google_autopost_token',NULL);
  
    // Check if token has been generated.
    // If token is not generated then ask user to authenticate the account.
    if (!isset($token)) {
     
      // Load google-api-php-client library.
      $info = libraries_load('google-api-php-client');
    
      // Check if library has been loaded.
      if (!$info['loaded']) {
        // Display informaiton for missing library.
        $form['lib_info'] = array(
          '#type' => 'item',
          '#title' => t('Google Client Library Missing'),
          '#markup' => t("Missing Google Client library. Check Status Reprot or Readme for requiremtents."),
          '#prefix' => '<p>',
          '#suffix' => '</p>',
        );
      }
      else {
        
        $client = _google_autopost_client($client_id, $client_secret, $callback_uri);
        $client->setApprovalPrompt('force');
        $auth_url = $client->createAuthUrl();

        // Authentication URL.
        $form['auth_url'] = array(
          '#type' => 'hidden',
          '#default_value' => $auth_url,
        );

        // Submit Button for Authentication.
        $form['auth'] = array(
          '#type' => 'submit',
          '#value' => t('Authorize Google+ Autopost'),
        ); 
      }    
    }
    else {
    
      // Token data.
      $token_data = drupal_json_decode($token['access_token']);
    
      // Token fieldset.
      $form['token'] = array(
        '#type' => 'fieldset',
        '#title' => t('Token Information'),
        '#description' => t('Token information since last authorized.'),
      ); 

      // Access Token value.
      $form['token']['access_token'] = array(
        '#type' => 'item',
        '#title' => t('Access Token'),
        '#markup' => isset($token_data['access_token']) ? $token_data['access_token'] : 'N/A',
        '#prefix'=> '<p>',
        '#suffix' => '</p>',
      );
   
      // Token type.
      $form['token']['token_type'] = array(
        '#type' => 'item',
        '#title' => t('Token Type'),
        '#markup' => isset($token_data['token_type']) ? $token_data['token_type'] : 'N/A',
        '#prefix'=> '<p>',
        '#suffix' => '</p>',
      );
   
      // Token expries in seconds.
      $form['token']['expires_in'] = array(
        '#type' => 'item',
        '#title' => t('Expires In'),
        '#markup' => isset($token_data['expires_in']) ? $token_data['expires_in'] : 'N/A',
        '#prefix'=> '<p>',
        '#suffix' => '</p>',
      );
   
      // Token ID.
      $form['token']['id_token'] = array(
        '#type' => 'item',
        '#title' => t('Token ID'),
        '#markup' => isset($token_data['id_token']) ? $token_data['id_token'] : 'N/A',
        '#prefix'=> '<p>',
        '#suffix' => '</p>',
      );
   
      // Refersh Token.
      $form['token']['refresh_token'] = array(
        '#type' => 'item',
        '#title' => t('Refresh Token'),
        '#markup' => isset($token_data['refresh_token']) ? $token_data['refresh_token'] : 'N/A',
        '#prefix'=> '<p>',
        '#suffix' => '</p>',
      );
    
      // Token Last Generated (Server Time).
      $form['token']['created'] = array(
        '#type' => 'item',
        '#title' => t('Created (Server Time)'),
        '#markup' => format_date($token_data['created']),
        '#prefix'=> '<p>',
        '#suffix' => '</p>',
      );
    }  
  }
    
  return $form;
}

function google_autopost_auth_form_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] == 'Authorize Google+ Autopost') {
     drupal_goto( $form_state['values']['auth_url'], array('external' => TRUE));
  }
}

/**
 * Page Callback for OAuth authentication.
 */
function google_autopost_oauth_callback() {
  
  $info = libraries_load('google-api-php-client');
  if (!$info['loaded']) {
    drupal_set_message(t("Can't authenticate with google as library is missing check <a href='@link'>Status Report</a> or README.txt for requirements", 
        array('@link' => url('admin/reports/status'))), 'error');
    return FALSE;
  }
  
  if (isset($_GET['code'])) {
    try {
    $client_id = variable_get('google_autopost_client_id', NULL);
    $client_secret = variable_get('google_autopost_client_secret', NULL);
    $callback_uri = variable_get('google_autopost_callback_uri', '');
    $client = _google_autopost_client($client_id, $client_secret, $callback_uri);
    $client->setApprovalPrompt('force');
    $client->authenticate($_GET['code']);
    $access_token = $client->getAccessToken();
    $tokens_decoded = json_decode($access_token);
    $refresh_token = $tokens_decoded->refresh_token;
    $data = array(
      'access_token' => $access_token,
      'refresh_token' => $refresh_token,
      'created' => REQUEST_TIME,
    );
    variable_set('google_autopost_token', $data);
    drupal_set_message(t('Google account has been succesfully authenticated.'), 'status');
    }
    catch (Exception $e) {
      drupal_set_message(t('An Error has occured: !data', 
          array('!data' => $e->getMessage())), 'error');
    }
    drupal_goto('admin/config/services/google-autopost/auth');
  }
}

/**
 * Helper function for Google client.
 * @param type $client_id string
 * @param type $client_secret string
 * @param type $callback_uri string
 * @return \Google_Client object
 */
function _google_autopost_client($client_id, $client_secret, $callback_uri){
    
  global $base_url;
    
  $client = new Google_Client();
  $client->setClientId($client_id);
  $client->setClientSecret($client_secret);
  $client->setRedirectUri($base_url . '/'. $callback_uri );
  $client->addScope(
    array(
      Google_Service_PlusDomains::PLUS_ME, 
      Google_Service_PlusDomains::PLUS_STREAM_WRITE
    )
  );
  $client->setAccessType("offline");
   
  return $client;
}