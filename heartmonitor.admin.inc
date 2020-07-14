<?php

/**
 * @file
 * Administrative page code for the Joinery Heart Monitor module.
 */

/**
 * Administrative settings.
 */
function heartmonitor_admin_settings() {
  $form['heartmonitor'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Joinery Heart Monitor server settings'),
    '#description'   => t('Heartbeat Monitor assumes that an external service is querying this site with intentionally nonexisten URLs to verify its uptime. At every cron run, Heartbeat Monitor will send an alert email if a suitable 404 message is not found in the log.'),
  );

  $form['heartmonitor']['string_404'] = array(
    '#type'          => 'textfield',
    '#title'         => t('String to search for in 404 URL'),
    '#default_value' => _heartmonitor_variable_get('string_404', ''),
    '#required'      => TRUE,
  );

  $form['heartmonitor']['min_age'] = array(
    '#type'          => 'textfield',
    '#attributes' => array(
      ' type' => 'number',
      'min' => '0',
      'step' => '1',
    ),
    '#title'         => t('Maximum age (minutes) of 404 message'),
    '#default_value' => _heartmonitor_variable_get('min_age', ''),
    '#required'      => TRUE,
    '#description'   => t('If 0, no checking is performed and no alerts are sent.'),
  );

  $form['heartmonitor']['recipient_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Alert recipient email'),
    '#default_value' => _heartmonitor_variable_get('recipient_email', ''),
    '#required'      => TRUE,
    '#description'   => t('Accepts multiple email addresses, separated by commas'),
  );

  $form['heartmonitor']['min_time'] = array(
    '#type'          => 'textfield',
    '#attributes' => array(
      ' type' => 'number',
      'min' => '0',
      'step' => '1',
    ),
    '#title'         => t('Minimum time (minutes) between email alerts'),
    '#default_value' => _heartmonitor_variable_get('min_time', ''),
    '#required'      => TRUE,
    '#description'   => t('If 0, no minimum will be enforced'),
  );

  $form = system_settings_form($form);
  return $form;
}

/**
 * Validation for the administrative settings form.
 */
function heartmonitor_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['recipient_email']) {
    $r_emails = str_replace(' ', '', $form_state['values']['recipient_email']);
    $pieces = explode(",", $r_emails);
    $errors = 0;

    foreach ($pieces as $email) {
      if (!valid_email_address($email)) {
        $errors++;
      }
    }

    if ($errors) {
      form_set_error('recipient_email', t('Invalid email address detected; please enter valid email addresses only.'));
    }
  }

  if ($form_state['values']['min_age'] < 0 || !ctype_digit($form_state['values']['min_age'])) {
    form_set_error('min_age', t('Invalid value detected; please enter valid value.'));
  }

  if ($form_state['values']['min_time'] < 0 || !ctype_digit($form_state['values']['min_time'])) {
    form_set_error('min_time', t('Invalid value detected; please enter valid value.'));
  }
}
